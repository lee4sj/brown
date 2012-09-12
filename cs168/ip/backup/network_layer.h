#ifndef NETWORK_LAYER_H
#define NETWORK_LAYER_H

#include <netinet/ip.h> 
#include "rip.h"
#include "util/htable.h"

#define MAX_PACKET_SIZE 65536

/*
 * Argument to pthread_create when starting the forwarding thread
 */
typedef struct nlayer_args {
   list_t *interface_lst;
   pthread_mutex_t *if_mutex;
} nlayer_args;

/*
 * Given the address of some payload, and the size of that payload, creates
 * an IP header using the argument protocol, and sends it using the UDP socket
 * and remote address from the argument interface.
 */
int send_packet(
      char *payload, 
      size_t size, 
      int protocol, 
      interface *to, 
      uint32_t saddr, 
      uint32_t daddr);

/*
 * Entry point for forwarding thread.  Argument is a pointer to the list
 * of interfaces.
 */
void *network_layer_init(void *arg);

/*
 * Handler is a function of one argument: a char * pointing to a buffer containing
 * an IP packet with this handler's protocol number.
 */
typedef void (*handler_t)(char *packet);

/*
 * Register the argument handler for the argument protocol number.
 */
void net_register_handler(uint8_t protocol_num, handler_t handler);


#endif
