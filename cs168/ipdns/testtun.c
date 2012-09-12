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
#include <firedns.h>
#include <glib.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <inttypes.h>

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

  fd_set readfds;
  FD_ZERO(&readfds);
  FD_SET(tunfd, &readfds);

  /* Now read data coming from the kernel */
  while(1) {

    select(tunfd + 1, &readfds, NULL, NULL, NULL);

    if (FD_ISSET(tunfd, &readfds)) {
      printf("got packet from virt. interface\n");

      unsigned char buffer[1500];
      int nread = read(tunfd,buffer,sizeof(buffer));
      if(nread < 0) {
        perror("Reading from interface");
        close(tunfd);
        exit(1);
      }

      char *encoded = encode_ip_packet(buffer, nread);
      //printf("encoded: %s\n", encoded);
      /*      int i;
      for (i = 0; i < strlen(encoded); i++) {
        printf("encoded[%d]: %u\n", i, encoded[i]);
      }
      printf("strlen(encoded: %d\n", strlen(encoded));*/

      char *suffix = ".d.dtun.co.cc";
      char dns_txt[strlen(encoded) + 14];
      //for (int i = 0; i < strlen(encoded); i++) {
        //dns_txt[i] = encoded[i];
      //}
      memcpy(dns_txt, encoded, strlen(encoded));
      memcpy(dns_txt + strlen(encoded), suffix, 13);
      dns_txt[strlen(encoded) + 13] = '\0';
      printf("dns_txt: %s\n", dns_txt);
      printf("strlen(dns_txt) = %d\n", strlen(dns_txt));

      //int firefd = firedns_gettxt(encoded);
      int firefd = firedns_gettxt(dns_txt);
      //int firefd = firedns_gettxt("a.d.dtun.co.cc");
      
      fd_set fire_fds;
      FD_ZERO(&fire_fds);
      FD_SET(firefd, &fire_fds);

      select(firefd + 1, &fire_fds, NULL, NULL, NULL);

      printf("got dns response\n");
      char *result = firedns_getresult(firefd);
      printf("response: %s\n", result);

    } else {
      printf("loop error\n");
      return 1;
    }

    /*
    int maxfd;
    if (tunfd > dnsfd)
      maxfd = tunfd + 1;
    else
      maxfd = dnsfd + 1;

    select(maxfd + 1, &readfds, NULL, NULL, NULL);

    if (FD_ISSET(dnsfd, &readfds)) {
      printf("got dns response\n");
      char *result = firedns_getresult(dnsfd);
      printf("response: %s\n", result);

      FD_CLR(dnsfd, &readfds);
      dnsfd = 0;
    } else if (FD_ISSET(tunfd, &readfds)) {
      printf("got ip packet from virtual interface\n");
      unsigned char buffer[1500];
      int nread = read(tunfd,buffer,sizeof(buffer));
      if(nread < 0) {
        perror("Reading from interface");
        close(tunfd);
        exit(1);
      }

      //char *result = encode_ip_packet(buffer, nread);
      //printf("encoded result: %s\n", result);
      //dnsfd = firedns_gettxt(result);
      dnsfd = firedns_gettxt("baseball.wp.dg.cx");
      FD_SET(dnsfd, &readfds);
      //free(result);
    }
    */

  }

  return 0;
}
