#ifndef ROUTE_H
#define ROUTE_H

#include <inttypes.h>

#include <time.h>
#include <pthread.h>

#include <netinet/in.h>

#include "rip.h"
#include "interface.h"

#include "util/htable.h"

#include "netlayer.h"

//an entry in a routing table
typedef struct routeentry
{
    uint8_t cost;             //cost in hops
    interface_t *interface;   //the interface through which to send packetsdirected to this destination address
    
    time_t lastRefreshTime;
} routeentry_t;

//the routing table for this process
//maps dest addresses to route structs
/*extern pthread_mutex_t g_routeMutex;
extern htable_t *g_routeTable;*/

//initialize and deinitialize routing. 
int route_init(interface_t **interfaces);
int route_exit();
int trigger_update();

interface_t *route_lookup(struct in_addr destAddress);

//send without any routing:
int net_send_noroute(struct ip* packet, interface_t *interface);

#endif
