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

#define BIND_PORT "53"
#define MAX_BUF 1500
#define MTU 200

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

  fd_set tunfds;

  for(;;) {

    char buffer[MAX_BUF];
    struct sockaddr_storage from;
    socklen_t fromlen = sizeof(struct sockaddr_in);
    bzero(&from, sizeof(struct sockaddr_in));

    struct dns_header k;

    int nrecv = recvfrom(sockfd, buffer, MAX_BUF, 0, (struct sockaddr *)&from, &fromlen);

    if(nrecv < 0){
      perror("recving error: ");
    }
    else{
      printf("packet received from the virtual interface\n");
      
      char *qname = buffer + sizeof(struct dns_header);
      int q_len = strlen(buffer + sizeof(struct dns_header));
      int name_size;
      char *name = parse_qname(qname, q_len + 1);
      


      name[strlen(name) - 13] = '\0';

      //      printf("name section of DNS packet: %s\n", qname);
      //      printf("name: %s\n", name);
      
      //      int newqlen;
      //      char *newqname = forqname(name, &newqlen);
      //printf("reformed qname len: %d\n", newqlen);
      //printf("reformed qname: %s\n", newqname);
      
      struct dns_header *header = buffer;
      uint16_t id = header->id;
      uint16_t tags = header->tags;

      //decode to IP packet                                
      unsigned int nbytes;
      char *decoded = decode_str(name, &nbytes);
      
      /*
      int kj = 0;
      for(kj = 0; kj < nbytes; kj++){
	printf("receivd [%d]: %hhu\n", kj, decoded[kj]);
      }
      */

      /*char *ran = (char*) alloca(100);
	sprintf(ran, "yoooooooooooooooooooooooooo");*/
      //      ran = "abcxy";

      //write to the tunnel
      int nwrite = write(tunfd, decoded, nbytes);
      //      printf("result of write 2: %d\n", nwrite);
      if(nwrite < 0){
	perror("writing to the virtual interface: ");
      }

      FD_ZERO(&tunfds);
      FD_SET(tunfd, &tunfds);

      select(tunfd + 1, &tunfds, NULL, NULL, NULL);

      //      printf("something wrote to tunnel\n");

      char tun_buffer[MTU];

      int nr = read(tunfd, tun_buffer, MTU);
      if(nr < 0) {
        perror("Reading from the virtual interface: ");
	/*close(sockfd);
        close(tunfd);                                                                                   
        exit(1);*/
      }
      //      printf("read %d bytes from the tunnel\n", nr);

      char* encoded = encode_ip_packet(tun_buffer, nr);

      //      printf("encoded: that's %s\n", encoded);

      //convert the packet to dns packet                                                                
      int packet_size;
      
      char * packet = dns_build_txt_packet(id, 0, 1, 1, 0, 0, qname, encoded, strlen(encoded)+1, &packet_size); 
      char to[INET_ADDRSTRLEN];
      inet_ntop(AF_INET, &from, to, INET_ADDRSTRLEN);
      
      printf("ID: %x\n", id);
      
      //send the response to the client                                                                 
      int res_send = sendto(sockfd, packet, packet_size, 0, (struct sockaddr *)&from, fromlen);
      if(res_send <= 0){
	perror("error in sending");
      }

      printf("done responding\n");

      free(name);
      free(encoded);
      free(decoded);
      free(packet);
    }

  }  
  return 0;
}
