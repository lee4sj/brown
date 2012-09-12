#ifndef PARSELINKS_H
#define PARSELINKS_H

#include <netinet/in.h>
#include <netinet/if_ether.h>

#include "util/list.h"

#define HOST_MAX_LENGTH 256 // RFC 2181

//look at "/usr/include/netinet/in.h" for a definition of struct in_addr.
typedef struct lnxlink {
  char local_phys_host[HOST_MAX_LENGTH];
  uint16_t local_phys_port;
  struct in_addr local_virt_ip;

  char remote_phys_host[HOST_MAX_LENGTH];
  uint16_t remote_phys_port;
  struct in_addr remote_virt_ip;

  list_link_t listnode;
} lnxline_t;

//parse_links returns a null-terminated array of link_t's. 
list_t *parse_links(char* filename);
void free_links(list_t *links);

#endif
