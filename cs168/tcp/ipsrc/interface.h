#ifndef INTERFACE_H
#define INTERFACE_H

#include <inttypes.h>
#include <netinet/in.h>

typedef struct interface {
/* BEGIN SOLUTION */
  struct sockaddr_in local_phys_addr;
  struct in_addr local_virt_ip;

  struct sockaddr_in remote_phys_addr;
  struct in_addr remote_virt_ip;

  int sock;

  //fault simulation:
  double droprate;
  uint32_t bandwidth;

  uint32_t lastSecond;
  uint32_t bytesSent;
  uint32_t bytesRcvd;
/* END SOLUTION */
} interface_t;

#endif
