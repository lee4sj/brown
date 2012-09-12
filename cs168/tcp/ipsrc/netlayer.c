#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#include <pthread.h>

#include "netlayer.h"
#include "route.h"
#include "linklayer.h"

#include "util/dbg.h"
#include "iputil.h"

//TODO: make this code thread safe, as interface->sock can change at any time.

//the global blocking queue where incoming ip packets are stored.
//there is one per protocol.
static pthread_mutex_t g_handler_mutex;
static handler_t g_handlers[256];
static handler_t g_default_handler = NULL;
static int g_inited = 0;
static interface_t **g_interfaces = NULL;

int net_init(interface_t **interfaces, handler_t default_handler)
{
    int i;
    pthread_t recvThreadID;    

    if (g_inited != 0)
    {
        dbg(DBG_NET, "net_init: IP already initialized.\n");
        return -1;
    }
    
    g_interfaces = interfaces;
    
    //init the handlers to NULL
    for (i=0; i < 256; ++i)
        g_handlers[i] = NULL;
    g_default_handler = default_handler;

    pthread_mutex_init(&g_handler_mutex, NULL);

    //everything's set.
    g_inited = 1;
    
    //start the receive threads, one per interface
    for (int i=0; interfaces[i]; ++i) {
        interface_t *interface = interfaces[i];
        if (interface->sock != -1)
	{
            dbg(DBG_NET, "Creating receive thread on interface from %s", 
                   inet_ntoa_host(interface->local_virt_ip));
            dbg(DBG_NET, " to %s\n", inet_ntoa_host(interface->remote_virt_ip));
            pthread_create(&recvThreadID, NULL, net_recv_thread, (void *)interfaces[i]);
	}
        else
	{
            dbg(DBG_NET, "Interface from %s ", inet_ntoa_host(interface->local_virt_ip));
            dbg(DBG_NET, "to %s is not up, not creating recv thread.\n", 
                   inet_ntoa_host(interface->remote_virt_ip));
	}
    }
    
    return 0;
}

int net_exit()
{
    int i;
    if (g_inited == 0)
    {
        dbg(DBG_NET, "net_exit: IP already deinitialized.\n");
        return -1;
    }
    
    //deinit the handlers
    for (i=0; i < 256; ++i)
        g_handlers[i] = NULL;

    //TODO: kill all threads. 
    g_interfaces = NULL;

    pthread_mutex_destroy(&g_handler_mutex);
    g_inited = 0;
    return 0;
}

int net_send(struct ip* packet)
{
    handler_t handler;
    int packetlen;
    for (int i=0; g_interfaces[i]; ++i)
    {
        if (g_interfaces[i]->local_virt_ip.s_addr == packet->ip_dst.s_addr)
        {
            if (g_interfaces[i]->sock == -1) 
            {
                dbg(DBG_NET, "net_send: packet is to %s, but that interface is down. discarding.\n",
                    inet_ntoa_host(packet->ip_dst));
                return -ENETUNREACH;
            }
            
            dbg(DBG_NET, "net_send: packet is to one of our interfaces.\n");
            //call the handler
            pthread_mutex_lock(&g_handler_mutex);
            handler = g_handlers[packet->ip_p];
            if (handler == NULL) handler = g_default_handler;
            pthread_mutex_unlock(&g_handler_mutex);
                
            if (handler == NULL)
                dbg(DBG_NET, "net_send: no handler for %d, no default handler.\n", packet->ip_p);
            else
                handler(g_interfaces[i], packet);
            
            packetlen=packet->ip_len;
            return packetlen;
        }
    }

    dbg(DBG_NET, "net_send: Forwarding packet to %s.\n", inet_ntoa_host(packet->ip_dst));
    
    interface_t *target = route_lookup(packet->ip_dst);
    if (target == NULL)
    {
        dbg(DBG_NET, "net_send: No route to virtual ip %s\n", inet_ntoa_host(packet->ip_dst));
        return -EHOSTUNREACH;
    }
    if (packet->ip_src.s_addr == 0)
      packet->ip_src.s_addr = target->local_virt_ip.s_addr;
    //we're forwarding this packet, so decrement the TTL:
    if (packet->ip_ttl != 0)
      packet->ip_ttl -= 1;
    int res = net_send_noroute(packet, target);
    packet->ip_ttl += 1;
    return res;
}

void net_register_handler(uint8_t protocol_num, handler_t handler)
{
  pthread_mutex_lock(&g_handler_mutex);
  if (g_handlers[protocol_num] != NULL)
    dbg(DBG_NET, "Warning: over-writing handler for protocol %d\n", protocol_num);

  g_handlers[protocol_num] = handler;
  pthread_mutex_unlock(&g_handler_mutex);
}

//receiving thread
void *net_recv_thread(void *arg)
{
    interface_t *interface = (interface_t *)arg;
    
    struct ip *packet;
    void *buf;
    int res;
    
    if (g_inited == 0)
    {
        dbg(DBG_NET, "net_recv_thread: net not initialized yet, exiting.\n");
        return NULL;
    }
        
    //get the MTU, allocate enough space for maximum input:
    buf = malloc(LINK_MTU);
    if (buf == NULL)
    {
      dbg(DBG_NET, "net_recv_thread: malloc failed, exiting.\n");
      return NULL;
    }
    
    //TODO: Should be a terminating condition here?
    while (1) {
        if (g_interfaces == NULL)
        {
            dbg(DBG_NET, "net_recv_thread: net is de-inited, exiting.\n");
            break;
        }
	res = link_recv(interface, buf, LINK_MTU);

        if (res < 0)
        {
	  if (interface->sock == -1)
          {
	      dbg(DBG_NET, "net_recv_thread: interface has gone down; quitting.\n");
	      break;
          }
          dbg(DBG_NET, "net_recv_thread: link_recv: %s\n", strerror(errno));
	  continue;
        }
        
        if (res == 0) //TODO: Not sure when this happens. interface down?
        {
	    dbg(DBG_NET, "net_recv_thread: link_recv returned 0. interface down?\n");
            break;
        }
        
        //construct the ip packet
        if ((unsigned int)res < sizeof(struct ip))
        {
            dbg(DBG_NET, "net_recv_thread: Packet discarded: Data received smaller than IP header.\n");
            continue;
        }
        
        packet = (struct ip *)malloc(sizeof(struct ip));
        memcpy(packet, buf, sizeof(struct ip));

        //see if it's correct by checking the checksum
        res = ip_calc_checksum(packet);
        if (!res)
        {
            dbg(DBG_NET, "net_recv_thread: Packet discarded: Invalid checksum.\n");
            free(packet);
            continue;
        }
        
        //convert packet to host order so we can use it
        ip_packet_ntoh(packet);
        
        //copy the data in to the packet 
        packet = (struct ip *)realloc(packet, packet->ip_len);
        memcpy(packet + 1, buf + sizeof(struct ip), 
	       packet->ip_len - sizeof(struct ip));
        
	dbg(DBG_NET, "net_recv_thread: packet recevied. Dest = %s.\n", inet_ntoa_host(packet->ip_dst));
        net_send(packet);
        free(packet);
    }
    
    free(buf);
    
    return NULL;
}
