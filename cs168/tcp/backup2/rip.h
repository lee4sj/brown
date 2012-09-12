#ifndef RIP_H
#define RIP_H

#include <inttypes.h>
#include <arpa/inet.h>
#include "util/list.h"


typedef struct interface {
  // For the up/down commands
  int id;
  int enabled;

  // These are needed for sendto/recvfrom calls
  int socketfd;
  struct sockaddr *to;
  socklen_t tolen;

  // Virtual ip addresses for the local and remote end
  // of the interface
  //in_addr_t local_virt_ip;
  //in_addr_t remote_virt_ip;
  struct in_addr local_virt_ip;
  struct in_addr remote_virt_ip;

  //for updates and expiration checks
  time_t time_stamp;


  list_link_t listnode;
} interface;

typedef struct rip{
  uint32_t network_addr;
  uint32_t cost;
  interface *next_hop;

  list_link_t ripnode;
}rip;


/*
 * Argument to rip_loop
 */
typedef struct rip_args {
   list_t *interface_lst;
   pthread_cond_t *ready;
   pthread_mutex_t *ready_mutex;
   int *ready_flag;
} rip_args;


/*
 * Function passed to pthread_create for forwarding thread
 */
//void *rip_loop(list_t *intrf_param);
void *rip_loop(void *arg);

/*
 * initializes rip.
 */
void rip_init(list_t *interface_list_param);

/*
 * The query method.
 * Given the network virtual ip address, return the nexthop (this case interface).
 */
interface *get_interface(struct in_addr *virt_ip);

/*
 * Handle a RIP packet for RIP table.
 * -1 is returned in case of failure to handle the packet or the invalid packet.
 */
int handle_rip_packet(char *packet, size_t size, struct in_addr *src);

/*
 * This function maintains the rip_table.
 * The function:
 *      1. At the start, send a request to each of its interfaces.
 *      2. Send the periodic updates to all of its interfaces every 5 seconds, and 
 *         check for the expired interfaces.
 */
int maintain_rip_table();

/*
 * sets all the network that goes through this interface to INFINITE.
 */
int interface_down(int id);

/*
 * send request to the neighbor connected to interface(id).
 */
int interface_up(int id);

/*
 * prints routing table information.
 */
int print_routes_info();

#endif
