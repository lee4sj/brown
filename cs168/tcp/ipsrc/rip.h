#ifndef RIP_H
#define RIP_H

#include <inttypes.h>

#include <netinet/in.h>

#define RIP_PROTOCOL_NUM    200 //protocol number to be used in ip packets containing rip packets

#define COMMAND_REQUEST     1 //request for all routing information the destination node has
#define COMMAND_RESPONSE    2 //response to a request from another node

#define ROUTE_INFINITY 16     //'infinite' cost to a destination = no route to destination

#define UPDATE_TIME      30
#define ENTRYEXPIRE_TIME 80

//look at "/usr/include/netinet/in.h" for a definition of struct in_addr.
typedef struct rip_entry
{
    uint32_t cost;          //must be <= ROUTE_INFINITY
    struct in_addr address; //virtual address
} rip_entry_t;

// Max route table entries per packet, enough to fit into the MTU for ethernet
#define MAX_RT_ENTRIES	(1400 / sizeof(rip_entry_t))

typedef struct rip_packet
{
    uint16_t command;
    uint16_t num_entries;   //up to MAX_RT_ENTRIES. 0 if command is COMMAND_REQUEST.
    rip_entry_t entries[0]; 
} rip_packet_t;

/* Example:
   To create a rip_packet_t containing 3 entries:
 
     rip_packet_t *packet = (rip_packet_t *)malloc(sizeof(rip_packet_t) + sizeof(rip_entry_t) * 3);
   
   Now you can use packet->entries like a regular array. The data will also be contiguous.
*/
   
#endif
