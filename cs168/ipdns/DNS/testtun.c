#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
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

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <inttypes.h>

#define DEFAULT_STR "d.dtun.co.cc"

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


char *get_suffix() {
  char *alphabet = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
  char *rtn = (char *)malloc(16);

  char *suffix = ".d.dtun.co.cc";

  int x;
  for (x = 0; x < 3; x++) {
    struct timespec tv;
    clock_gettime(CLOCK_REALTIME, &tv);
    int i = (char)((tv.tv_nsec) % 62);
    rtn[x] = alphabet[i];
  }

  memcpy(rtn + 3, suffix, 13);
  return rtn;
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

  fd_set masterfds;
  fd_set readfds;
  FD_ZERO(&readfds);
  FD_ZERO(&masterfds);

  FD_SET(tunfd, &masterfds);
  printf("added tunfd = %d to masterfds\n", tunfd);
  int maxfd = tunfd;


  /* Now read data coming from the kernel */
  while(1) {

    // 1 second loop
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 500000;

    readfds = masterfds;
    int srv = select(maxfd + 1, &readfds, NULL, NULL, &tv);
    //printf("srv = %d\n", srv);

    if (srv < 0) {
      perror("select: ");
      return -1;
    }

    int i;
    for (i = 0; i < maxfd + 1; i++) {
      if (FD_ISSET(i, &readfds)) {
        if (i == tunfd) {
          //printf("got packet from virt. interface\n");

          unsigned char buffer[1500];
          int nread = read(tunfd,buffer,sizeof(buffer));
          if(nread < 0) {
            perror("Reading from interface");
            close(tunfd);
            exit(1);
          }

          //printf("tunnel read %d bytes\n", nread);

          char *encoded = encode_ip_packet(buffer, nread);

          //char *suffix = ".d.dtun.co.cc";

          char *suffix = get_suffix();
          //printf("get_suffix returned %s\n", suffix);

          char dns_txt[strlen(encoded) + 18];

          memcpy(dns_txt, encoded, strlen(encoded));
          dns_txt[strlen(encoded)] = '.';
          memcpy(dns_txt + strlen(encoded) + 1, suffix, 16);
          dns_txt[strlen(encoded) + 17] = '\0';
          //printf("dns_txt = %s\n", dns_txt);
          //memcpy(dns_txt + strlen(encoded), suffix, 13);
          //dns_txt[strlen(encoded) + 13] = '\0';

          int firefd = firedns_gettxt(dns_txt);
          if (firefd < 0) {
            printf("firefd negative: %d\n", firefd);
            return -1;
          }

          if (firefd > maxfd)
            maxfd = firefd;

          //printf("firefd = %d, adding to master set\n", firefd);
          FD_SET(firefd, &masterfds);

        } else {
          //printf("got response from firedns: i = %d\n", i);
          char *result = firedns_getresult(i);

          if (result != NULL) {
            int rlen;
            char *rpacket = decode_str(result, &rlen);
            //printf("encoded to %d bytes\n", rlen);

            if (rlen > 0) {
              int wr = write(tunfd, rpacket, rlen);
              //printf("wrote %d bytes to tunnel\n", wr);
            }

          }

          FD_CLR(i, &masterfds);
          //printf("removed %d from master set\n", i);

          if (i == maxfd)
            maxfd -= 1;
        }

        //printf("\n");
      }
    }

    if (srv == 0) {
      //printf("keep alive\n");
      //int firefd = firedns_gettxt(DEFAULT_STR);
      char *default_str = get_suffix();
      //printf("keep alive str: %s\n", default_str);
      int firefd = firedns_gettxt(default_str);

      if (firefd < 0) {
        printf("firefd negative: %d\n", firefd);
        return -1;
      }

      if (firefd > maxfd)
        maxfd = firefd;

      //printf("keepalive firefd = %d, adding to master set\n", firefd);
      FD_SET(firefd, &masterfds);
    }
  }

  return 0;
}
