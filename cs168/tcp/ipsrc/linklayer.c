#include "linklayer.h"

/* BEGIN SOLUTION */
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <stdio.h>
#include "util/dbg.h"
#include <errno.h>
#include <time.h>
/* END SOLUTION */

#include "interface.h"
/* BEGIN SOLUTION */
int link_up(interface_t *netif){
  int ret;

  netif->sock = socket(AF_INET, SOCK_DGRAM, 0);
  if (netif->sock == -1){
    dbg(DBG_LINK, "link_up: socket");
    return -1;								
  }									
  int yes = 1;
  ret = setsockopt(netif->sock, SOL_SOCKET, SO_REUSEADDR,&yes,sizeof(int));
  if (ret == -1) {
    dbg(DBG_LINK, "link_up: setsockopt: %s\n", strerror(errno));
    return -1;
  }
  ret = bind(netif->sock, (struct sockaddr *)&netif->local_phys_addr,
             sizeof(netif->local_phys_addr)); 
  if (ret == -1){
    dbg(DBG_LINK, "link_up: bind: %s\n", strerror(errno));
    netif->sock = -1;
    return -1;
  }
  ret = connect(netif->sock, (struct sockaddr *)&netif->remote_phys_addr,
                sizeof(netif->remote_phys_addr));
  if (ret == -1){
    dbg(DBG_LINK, "link_up: connect: %s\n", strerror(errno));
    netif->sock = -1;
    return -1;
  }
  return 0;
}

int link_down(interface_t *netif){
  int ret = close(netif->sock);
  netif->sock = -1;
  return ret;
}
/* END SOLUTION */

ssize_t link_send(interface_t* netif, const char* buf, size_t len) {
  /* BEGIN SOLUTION */
  int res;
  if (len > LINK_MTU)
  {
    dbg(DBG_LINK, "link_send: not sending, %d is > the MTU, %d.\n",
        len, LINK_MTU);
    return -EMSGSIZE;
  }
  if (drand48() < netif->droprate) {
    dbg(DBG_LINK, "link_send: Dropped random packet.\n");
    return len;
  }

  if (netif->bandwidth != 0) {
    uint32_t t = time(NULL);
    if (t != netif->lastSecond) {
      netif->bytesSent = 0;
      netif->bytesRcvd = 0;
      netif->lastSecond = t;
    }

    if (netif->bytesSent + len > netif->bandwidth) {
      dbg(DBG_LINK, "link_send: Dropped packet due to exceeded outgoing bandwidth.\n");
      return len;
    }
    netif->bytesSent += len;
  }

  res = write(netif->sock, buf, len);
  if (res < 0)
  {
    dbg(DBG_LINK, "link_send: Error writing %d bytes\n", len);
    return -errno;
  }
  return res;
  /* END SOLUTION */
  return len;
}

ssize_t link_recv(interface_t* netif, char* buf, size_t len) {
  /* BEGIN SOLUTION */
  int res = read(netif->sock, buf, len);
  if (res < 0)
  {
    dbg(DBG_LINK, "link_recv: Error reading %d bytes\n", len);
    return -errno;
  }

  if (netif->bandwidth != 0) {
    uint32_t t = time(NULL);
    if (t != netif->lastSecond) {
      netif->bytesSent = 0;
      netif->bytesRcvd = 0;
      netif->lastSecond = t;
    }
    netif->bytesRcvd += res;

    if (netif->bytesRcvd > netif->bandwidth) {
      dbg(DBG_LINK, "link_recv: Dropped packet due to exceeded incoming bandwidth.\n");
      return res;
    }
  }

  return res;
  /* END SOLUTION */
  return 0;
}


