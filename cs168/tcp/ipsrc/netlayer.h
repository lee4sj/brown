#ifndef NETLAYER_H
#define NETLAYER_H

#include <inttypes.h>
#include <sys/types.h>

#define __USE_BSD
typedef uint16_t u_short; //necessary due to strange types in "netinet/ip.h"
#include <netinet/ip.h>

#include "interface.h"
typedef void (*handler_t)(interface_t *, struct ip *);

/* BEGIN SOLUTION_LEAVEOUT */
int net_init(interface_t **interfaces, handler_t default_handler);
int net_exit();
//the thread that accepts all incoming IP packets and calls the proper handlers for them
void *net_recv_thread(void *arg);
/* END SOLUTION_LEAVEOUT */

int net_send(struct ip* packet);
void net_register_handler(uint8_t protocol_num, handler_t handler);

#endif
