#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <net/if.h>
#include <linux/if_tun.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <arpa/inet.h> 
#include <netinet/ip.h>
#include <sys/select.h>
#include <sys/time.h>
#include <errno.h>
#include <stdarg.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <inttypes.h>

#include "base64.h"
#include "dns_builder.h"
#include "list.h"

#define BIND_PORT "53"
#define MAX_BUF 1500
#define MY_MTU 200
#define MAX_DNS_PACK 3


typedef struct id_holder_struct{
  uint16_t id;
  char* qname;
  struct sockaddr_storage from;

  list_link_t id_listnode;
} id_holder;

/**************************************************************************
 * tun_alloc: allocates or reconnects to a tun/tap device. The caller     *
 *            must reserve enough space in *dev.                          *
 **************************************************************************/
int tun_alloc(char *dev, int flags) {

  struct ifreq ifr;
  int fd, err;
  char *clonedev = "/dev/net/tun";

  if( (fd = open(clonedev , O_RDWR)) < 0 ) {
    perror("Opening /dev/net/tun");
    return fd;
  }

  memset(&ifr, 0, sizeof(ifr));

  ifr.ifr_flags = flags;

  if (*dev) {
    strncpy(ifr.ifr_name, dev, IFNAMSIZ);
  }

  if( (err = ioctl(fd, TUNSETIFF, (void *)&ifr)) < 0 ) {
    perror("ioctl(TUNSETIFF)");
    close(fd);
    return err;
  }

  strcpy(dev, ifr.ifr_name);

  return fd;
}

int main (int argc, char **argv) {

  if (argc != 2) {
    printf("usage: tuntest <device> \n");
    return -1;
  }

  char *device = argv[1];

  char tun_name[IFNAMSIZ];

  /* Connect to the device */
  strcpy(tun_name, device);
  int tunfd = tun_alloc(tun_name, IFF_TUN | IFF_NO_PI);  /* tun interface */

  if(tunfd < 0){
    perror("Allocating interface");
    exit(1);
  }

  //1. get local address information.                                                                   
  struct addrinfo hints, *res;
  int sockfd, bindres;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_flags = AI_PASSIVE;

  getaddrinfo(NULL, BIND_PORT, &hints, &res);

  sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  if(sockfd < 1){
    perror("socket error: ");
    exit(0);
  }
  
  bindres = bind(sockfd, res->ai_addr, res->ai_addrlen);
  if(bindres < 0){
    perror("bind error: ");
    exit(0);
  }
  /* Now read data coming from the kernel */

  fd_set sockfds;
  int maxfd = sockfd;
  if(sockfd < tunfd)
    maxfd = tunfd;

  socklen_t fromlen = sizeof(struct sockaddr_storage);

  list_t *id_list = (list_t *)malloc(sizeof(list_t));
  list_init(id_list);

  int dns_pack_counter;

  for(;;) {

    //setup the fd_sets
    FD_ZERO(&sockfds);

    FD_SET(sockfd, &sockfds);
    FD_SET(tunfd, &sockfds);

    //select!
    /*struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;*/

    int srs = select(maxfd + 1, &sockfds, NULL, NULL, NULL);
    if(srs < 0)
      perror("error in select: ");      
    if(FD_ISSET(tunfd, &sockfds)){

      if(!list_empty(id_list)){
	//	printf("hello\n");
	list_link_t *link = id_list->l_next;
	id_holder *ids = list_item(link, id_holder, id_listnode);
	
	uint16_t id = ids->id;
	char* qname = ids->qname;
	struct sockaddr_storage from = ids->from;
	
	list_remove_head(id_list);
	dns_pack_counter--;
	
	printf("packet received from the virtual interface\n");
	char buffer[MY_MTU];
	bzero(buffer, MY_MTU);
	
	int nr = read(tunfd, buffer, MY_MTU);
	if(nr < 0) {
	  perror("Reading from the virtual interface: ");
	  /*close(sockfd);
	    close(tunfd);                                                                                   
	    exit(1);*/
	}
	
	char* encoded = encode_ip_packet(buffer, nr);
	
	//convert the packet to dns packet                                                                
	int packet_size;
	
	char * packet = dns_build_txt_packet(id, 0, 1, 1, 0, 0, qname, encoded, strlen(encoded) + 1, &packet_size); 
	
	//send the response to the client                                                                 
	int res_send = sendto(sockfd, packet, packet_size, 0, (struct sockaddr *)&from, fromlen);
	
	
	if(res_send <= 0){
	  perror("error in sending");
	}
	
	free(qname);
	free(ids);
	
	free(encoded);
	free(packet);
      }
    }
    
    else {//if(dns_pack_counter > MAX_DNS_PACK){
      if(!list_empty(id_list)){
	list_link_t *link = id_list->l_next;
	id_holder *ids = list_item(link, id_holder, id_listnode);
	
	uint16_t id = ids->id;
	char* qname = ids->qname;
	struct sockaddr_storage from = ids->from;
	
	list_remove_head(id_list);
	dns_pack_counter--;
	
	printf("keep alive\n");
	
	//convert the packet to dns packet                                                                
	int packet_size;
	
	char * packet = dns_build_txt_packet(id, 0, 1, 1, 0, 0, qname, NULL, 0, &packet_size); 
	
	//send the response to the client                                                                 
	int res_send = sendto(sockfd, packet, packet_size, 0, (struct sockaddr *)&from, fromlen);
	if(res_send <= 0){
	  perror("error in sending");
	}
	
	free(qname);
	free(ids);
	free(packet);
      }
    }
  

    
    if(FD_ISSET(sockfd, &sockfds)){
      printf("sockfd\n");
      char buffer[MAX_BUF];
      bzero(buffer, MAX_BUF);
      struct sockaddr_storage from;
      bzero(&from, sizeof(struct sockaddr_in));
      
      int nrecv = recvfrom(sockfd, buffer, MAX_BUF, 0, (struct sockaddr *)&from, &fromlen);

      if(nrecv < 0){
	perror("recving error: ");
      }
      else{
	printf("dns query received!\n");
	
	char *qname = buffer + sizeof(struct dns_header);
	int q_len = strlen(buffer + sizeof(struct dns_header));
	int name_size;
	char *name = parse_qname(qname, q_len + 1);
	
	//store the qname into ;
	char *qname_store = (char *) malloc(q_len + 1 + 4);
	memcpy(qname_store, qname, q_len + 1 + 4);
	name[strlen(name) - 13] = '\0';
	
	struct dns_header *header = buffer;
	uint16_t id = header->id;
	uint16_t tags = header->tags;
	
	id_holder *to_add = (id_holder *)malloc(sizeof(id_holder));
	to_add->id = id;
	to_add->qname = qname_store;
	to_add->from = from;
	
	list_insert_tail(id_list, &to_add->id_listnode);
	dns_pack_counter++;

	printf("%d\n", dns_pack_counter);

	if(q_len != 13){

	  //decode to IP packet                                
	  unsigned int nbytes;
	  char *decoded = decode_str(name, &nbytes);
	  
	  //write to the tunnel
	  int nwrite = write(tunfd, decoded, nbytes);
	  if(nwrite < 0){
	    perror("writing to the virtual interface: ");
	  }
	  
	  free(decoded);
	  free(name);
	  
	}
      }
    }

  }
  return 0;
}
