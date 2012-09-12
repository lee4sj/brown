#ifndef LINK_LAYER_H
#define LINK_LAYER_H

#include <pthread.h>
#include "rip.h"


/*
 * Initialize the link layer.  Argument is a pointer to the list
 * of interfaces, and a pointer to the mutex protecting the list.
 */
void link_layer_init(list_t *arg, pthread_mutex_t *if_mutex);

/*
 * Given the address of a 64k buffer, blocks until an IP packet has been recieved
 * on some interface.  Then, writes that packet to the buffer, and returns the
 * size of the packet in bytes.
 */
int get_packet(char *buffer);

/*
 * Given the address of an IP packet with header, and the size of the packet,
 * sends that packet using the UDP socket and remote address from the argument
 * interface.
 */
int link_send_packet(char *packet, size_t size, interface *to);

#endif
