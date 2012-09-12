#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <time.h>
#include <assert.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <errno.h>

#include "route.h"
#include "netlayer.h"
#include "linklayer.h"
#include "util/dbg.h"
#include "iputil.h"

//the routing table for this process
pthread_mutex_t g_routeMutex;
htable_t *g_routeTable = NULL;

// The trigger g_update boolean
pthread_mutex_t g_updateMutex;
int g_update = 0;

static interface_t **g_interfaces = NULL;

void rip_packet_hton(rip_packet_t *packet)
{
  int i;
  for (i=0; i < packet->num_entries; ++i)
  {
    packet->entries[i].cost = htonl(packet->entries[i].cost);
    packet->entries[i].address.s_addr = htonl(packet->entries[i].address.s_addr);
  }
  packet->command = htons(packet->command);
  packet->num_entries = htons(packet->num_entries);
}

void rip_packet_ntoh(rip_packet_t *packet)
{
  int i;
  packet->command = ntohs(packet->command);
  packet->num_entries = ntohs(packet->num_entries);
  for (i=0; i < packet->num_entries; ++i)
  {
    packet->entries[i].cost = ntohl(packet->entries[i].cost);
    packet->entries[i].address.s_addr = ntohl(packet->entries[i].address.s_addr);
  }
}

int trigger_update()
{
  pthread_mutex_lock(&g_updateMutex);
  g_update = 1;
  pthread_mutex_unlock(&g_updateMutex);
  return 1;
}

//announce our routing table to one interface
//TODO: refactor this code / re-write this function
int announce_routes(interface_t *interface) 
{
  unsigned int i, j, packetlen;
  int res;
  struct ip *ip_pack;
  //just allocate enough room for the max possible packet size:
  rip_packet_t *packet = (rip_packet_t *)malloc(4 + 8 * MAX_RT_ENTRIES);
  routeentry_t *entry; 
    
  int hashkey;
    
  struct in_addr fromAddr = interface->local_virt_ip, 
    destAddr = interface->remote_virt_ip;

  if (interface == NULL)
    return -1;
    
  //uint32_t fwdAddress;

  if (interface->sock == -1)
  {
    dbg(DBG_ROUTE, "Not updating %s, interface is down.\n",
        inet_ntoa_host(interface->local_virt_ip));
    return -1;
  }
  dbg(DBG_ROUTE, "Sending routing table to interface %s\n", 
      inet_ntoa_host(interface->local_virt_ip));

  // Iterate through the keys of the hash table
  i = 0;
  j = 0;

  pthread_mutex_lock(&g_routeMutex);    
  packet->command = COMMAND_RESPONSE;
  dbg(DBG_ROUTE, "Sending update to %s\n", inet_ntoa_host(destAddr));
  dbg(DBG_ROUTE, "route-table has size %d\n", g_routeTable->ht_size);
  packet->num_entries = g_routeTable->ht_size;
  if (packet->num_entries >= MAX_RT_ENTRIES)
    packet->num_entries = MAX_RT_ENTRIES;

  htable_iterate_begin(g_routeTable, hashkey, entry, routeentry_t) {
    if (j >= MAX_RT_ENTRIES)
      // Send the current packet out
      // Create new packets
    {
      packetlen = 4 + (8 * packet->num_entries);
      dbg(DBG_ROUTE, "Sending a packet with %d entries\n", packet->num_entries);
      rip_packet_hton(packet);
      ip_pack = ip_packet_construct(
        fromAddr, destAddr, 0, 1, 
        RIP_PROTOCOL_NUM, (void *)packet, packetlen);
      // Send this packet to the given interface
      res = net_send_noroute(ip_pack, interface);
      ip_packet_destruct(ip_pack);
            
      if (res < 0)
      {
        dbg(DBG_ROUTE, "announce_routes: Failed to send packet to %s: %s\n",
            inet_ntoa_host(destAddr), strerror(-res));
        pthread_mutex_unlock(&g_routeMutex);
        free(packet);
        return -1;
      }
            
      rip_packet_ntoh(packet);

      // Create the new packet
      j = 0;
      packet->command = COMMAND_RESPONSE;
      packet->num_entries = g_routeTable->ht_size - i;
      if (packet->num_entries >= MAX_RT_ENTRIES)
        packet->num_entries = MAX_RT_ENTRIES;
    }
          
    packet->entries[j].cost = entry->cost;
    packet->entries[j].address.s_addr = hashkey; //the key is actually the IP address
        
    dbg(DBG_ROUTE, "    cost=%d, fwd=%s\n", packet->entries[j].cost,
        inet_ntoa_host(packet->entries[j].address));
          
    // Split horizon:
    if (entry->interface != NULL && 
        destAddr.s_addr == entry->interface->remote_virt_ip.s_addr)
    {
      //uncomment one of the following:
      
      ////With poisoned reverse:
      packet->entries[j].cost = ROUTE_INFINITY;
      //without:
      //j--;
      //packet->num_entries -= 1;
    }
          
    j++;
    i++; 
  } htable_iterate_end(); 
    
  //by here we're done with the routing table
  pthread_mutex_unlock(&g_routeMutex);
    
  //send remainder packet out
  res = 0;
  if (packet->num_entries > 0) {
    packetlen = 4 + (8 * packet->num_entries);
    rip_packet_hton(packet);
    ip_pack = ip_packet_construct(fromAddr, destAddr, 0, 1, RIP_PROTOCOL_NUM, 
                                  (void *)packet, packetlen);
    // Send this packet to the given interface
    res = net_send_noroute(ip_pack, interface);
    ip_packet_destruct(ip_pack);
  }
    
  free(packet);
  if (res < 0)
  {
    dbg(DBG_ROUTE, "announce_routes: Failed to send packet to %s: %s\n", 
        inet_ntoa_host(destAddr), strerror(-res));
    return -1;
  }

  return 0;
}

//announce_routes to all interfaces
int announce_all(interface_t **interfaces)
{    
  interface_t **curint;
  dbg(DBG_ROUTE, "Sending routing table to everyone.\n");
    
  for (curint=&interfaces[0]; *curint; curint++)
  {
    if (announce_routes(*curint) == -1)
    {
      dbg(DBG_ROUTE, "Failed to send a response on interface to %s\n", 
          inet_ntoa_host((*curint)->remote_virt_ip));
    }
  }
  return 0;
}

//update the routing table with the given information
//destAddress and cost represent the node we can route to, and at what cost
//supplierInterface is the interface of the person we're directly connected to that told us
//about this route.

//if there is no entry, then an entry is added and 1 is returned
//if the entry in the routing table is already better, 0 is returned
//if the given information is better, the table is updated and 1 is returned.
//NOTE: this DOES NOT lock the routing table mutex.
//printmode=0 to print a full line ("route: this happened"), 1 to print shortened line.
int update_routing_table(struct in_addr destAddress, uint8_t cost, interface_t *supplier_int, int printmode)
{
  routeentry_t *curentry, *newentry;

  if (cost >= ROUTE_INFINITY) cost = ROUTE_INFINITY;

  curentry = (routeentry_t *)htable_get(g_routeTable, destAddress.s_addr);
  if (curentry != NULL)
  {
    //if the supplier is the same...
    if (curentry->interface == supplier_int)
    {
      //if the cost is the same, we're verifying an existing entry.
      if (curentry->cost == cost) {
        if (printmode==0)
          dbg(DBG_ROUTE, "route: Refreshing entry for %s, cost still %d.\n", 
              inet_ntoa_host(destAddress), curentry->cost);
        else
          dbg(DBG_ROUTE, "refreshed to %d.\n", curentry->cost);
        curentry->lastRefreshTime = time(NULL);       
        return 0;         
      }

      //otherwise, we're updating it - a route changed / link went down, etc.
      //NOTE: This seems to fix problems with split horizon
      //since if a node you connected to suddenly lost route, you also assume
      //you lose route unless you get an update from someone else.
      if (curentry->cost > cost) {
        if (printmode==0)
          dbg(DBG_ROUTE, "route: Found better route to %s from %d to %d.\n", 
              inet_ntoa_host(destAddress), curentry->cost, cost);
        else
          dbg(DBG_ROUTE, "improved cost to %d\n", curentry->cost);
        curentry->cost = cost;
        curentry->lastRefreshTime = time(NULL);
        return 1;
      }
	    
      /*if (curentry->cost == 1) {
        dbg(DBG_ROUTE, "route: Not worsening route of cost 1 - direct link.\n");
        return 0;
        }*/ 
	    
      if (printmode==0)
        dbg(DBG_ROUTE, "route: Route to %s worsened from %d to %d\n", 
            inet_ntoa_host(destAddress), curentry->cost, cost);
      else
        dbg(DBG_ROUTE, "worsened from %d\n", curentry->cost);
      curentry->cost = cost;
      curentry->lastRefreshTime = time(NULL);
            
      return 1;
    }
        
    //if it's not, maybe we have a better route from somewhere else.
    if (curentry->cost > cost)
    {
      if (printmode==0)
        dbg(DBG_ROUTE, "route: Found better route to %s from a different interface, cost from %d to %d.\n", 
            inet_ntoa_host(destAddress), curentry->cost, cost);
      else
        dbg(DBG_ROUTE, "improved from %d (diff route)\n", curentry->cost);
      //we found a better route
      // Replace the old value with the new value
      curentry->cost = cost;
      curentry->interface = supplier_int;
      curentry->lastRefreshTime = time(NULL);
      return 1;
    }
    else
    {
      if (printmode==0)
        dbg(DBG_ROUTE, "route: Not updating with entry from different interfaces, ours is <= it.\n");
      else
        dbg(DBG_ROUTE, ">= ours, not updating\n");
    }
  }
  else
  {
    if (printmode==0)
      dbg(DBG_ROUTE, "route: Found new route to %s, cost=%d.\n", inet_ntoa_host(destAddress), cost);
    else
      dbg(DBG_ROUTE, "new route\n");
    //we found a new route.
    // Put in a new value
    free(curentry);
    newentry = (routeentry_t *)malloc(sizeof(routeentry_t));
    newentry->interface = supplier_int;
    newentry->cost = cost;
    newentry->lastRefreshTime = time(NULL);
    htable_put(g_routeTable, destAddress.s_addr, newentry);
    return 1;
  }
  return 0;    
}

void rip_handler(interface_t *interface, struct ip *packet)
{
  int i;

  rip_packet_t *rip_pack;
    
  int triggerUpdate;
    
  if (packet == NULL)
  {
    dbg(DBG_ROUTE, "rip_handler: received NULL packet");
    return;
  }

  dbg(DBG_ROUTE, "rip_handler: Received packet from %s",
      inet_ntoa_host(interface->remote_virt_ip));
  dbg(DBG_ROUTE, ", says it's from %s\n", inet_ntoa_host(packet->ip_src));
  //ip_packet_print(packet, 1);

  //now deal with the packet.
  triggerUpdate = 0;
        
  //update the routing table with information directly connecting us to
  //this node.
  //the packet's source might be wrong, but the interface is always right
  pthread_mutex_lock(&g_routeMutex);
  if (update_routing_table(interface->remote_virt_ip, 1, interface, 0))
    triggerUpdate = 1;
  pthread_mutex_unlock(&g_routeMutex);

  rip_pack = (rip_packet_t *)(packet + 1); //data starts after the header
  rip_packet_ntoh(rip_pack);
    
  if (rip_pack->command == COMMAND_REQUEST)
  {
    // This is a request, so send back a response
    dbg(DBG_ROUTE, "route: Request received from %s.\n", 
        inet_ntoa_host(packet->ip_src));
    announce_routes(interface);
  }
  else if (rip_pack->command == COMMAND_RESPONSE)
  {
    uint32_t cost;
    struct in_addr address;
    // This is a response, parse it and update
    dbg(DBG_ROUTE, "route: Response received from %s:\n", 
        inet_ntoa_host(packet->ip_src));

    pthread_mutex_lock(&g_routeMutex);
    for (i=0; i < rip_pack->num_entries; i++)
    {
      cost    = rip_pack->entries[i].cost;
      address = rip_pack->entries[i].address;

      int i;
      int ignore=0;
      for (i=0; g_interfaces[i]; ++i)
      {
        if (address.s_addr == g_interfaces[i]->local_virt_ip.s_addr)
        {
          dbg(DBG_ROUTE, "  Ignoring an entry about ourselves.\n");
          ignore=1;
          break;
        }
      }
      if (ignore) continue;
        
      dbg(DBG_ROUTE, "  Cost: %d. Address: %s, ", cost, inet_ntoa_host(address));
                
      //update the routing table
      //dbg(DBG_ROUTE, "  ");
      if (update_routing_table(address, cost+1, interface, 1))
        triggerUpdate = 1;
    }
    pthread_mutex_unlock(&g_routeMutex);
  }
  else
  {
    dbg(DBG_ROUTE, "handleRequests: unknown RIP packet command from %s: %d\n",
        inet_ntoa_host(packet->ip_src), rip_pack->command);
  }

  if (triggerUpdate)
    trigger_update();
}

//helper function, since it's called in multiple locations
void update_own_interfaces(interface_t **interfaces) {
  dbg(DBG_ROUTE, "route: updating routing table with own interfaces\n");
  for (int i=0; interfaces[i]; i++)
  {
    if (interfaces[i]->sock == -1) 
      update_routing_table(interfaces[i]->local_virt_ip, ROUTE_INFINITY,
                           NULL, 0);
    else
      update_routing_table(interfaces[i]->local_virt_ip, 0, NULL, 0);
  }      
}

void *update_thread(void *arg)
{
  interface_t **interfaces = (interface_t **)arg;
  int updateCounter;
  rip_packet_t *packet = (rip_packet_t *)malloc(4 + 8 * MAX_RT_ENTRIES);
    
  struct ip *ip_pack;
    
  routeentry_t *entry;
    
  int update, htsize;
    
  int triggerUpdate;
   
  //first, send out requests:
  dbg(DBG_ROUTE, "route: sending out requests to all our interfaces\n");
  interface_t **curintp;
  packet->command = COMMAND_REQUEST;
  packet->num_entries = 0;
  rip_packet_hton(packet);
  int packetlen = 4;
  for (curintp = &interfaces[0]; *curintp; ++curintp) 
  {
    interface_t *curint = *curintp;
    ip_pack = ip_packet_construct(curint->local_virt_ip, curint->remote_virt_ip, 
                                  0, 1, RIP_PROTOCOL_NUM, (void *)packet, packetlen);
    if (net_send_noroute(ip_pack, curint) < 0)
    { 
      dbg(DBG_ROUTE, "update_thread: Failed to send request packet on interface %s\n",
          inet_ntoa_host(curint->local_virt_ip));
    }
    ip_packet_destruct(ip_pack);
  }
  rip_packet_ntoh(packet);
   
  updateCounter = 0;
  while (1)
  {
    // Check for triggered updates
    pthread_mutex_lock(&g_updateMutex);
    update = g_update;
    pthread_mutex_unlock(&g_updateMutex);
    //printf("done checking.\n");
    if (update == 1) // Send your data to everyone
    {
      dbg(DBG_ROUTE, "route: Triggered update.\n");
      if (announce_all(interfaces) != 0)
      {
        dbg(DBG_ROUTE, "update_thread: announce_all has encountered an error\n");
      }            
    }

    if (updateCounter == 0 || update==1) // Do updates every UPDATE_TIME seconds
    {            
      // Send updates to everyone
      pthread_mutex_lock(&g_routeMutex);
      htsize = g_routeTable->ht_size;
      pthread_mutex_unlock(&g_routeMutex);
      if (htsize != 0) // Do not send it if it is empty
      {
        dbg(DBG_ROUTE, "route: %s update.\n", (updateCounter==0?"Regular":"Triggered"));
              
        if (announce_all(interfaces) != 0)
        {
          dbg(DBG_ROUTE, "route: %dsec update: announce_all has encountered an error\n", UPDATE_TIME);
        }
      }
      else
        dbg(DBG_ROUTE, "route: no entries in table, not updating.\n");
          
      //schedule next update
      updateCounter = -UPDATE_TIME;
      if (update == 1)
      {
        pthread_mutex_lock(&g_updateMutex);
        g_update = 0;
        pthread_mutex_unlock(&g_updateMutex);
      }
      update_own_interfaces(interfaces);
    }
        
    //check the table for expired entries and such.
    pthread_mutex_lock(&g_routeMutex);
    triggerUpdate = 0;
    int key;
    htable_iterate_begin(g_routeTable, key, entry, routeentry_t) {
      if (time(NULL) - entry->lastRefreshTime >= ENTRYEXPIRE_TIME)
      {
        if (entry->cost != ROUTE_INFINITY)
        {
          dbg(DBG_ROUTE, "route: Route to %d expired.\n", __hnode->hn_id);
          entry->cost = ROUTE_INFINITY;
          triggerUpdate = 1;
        }
      }
    } htable_iterate_end();        
    pthread_mutex_unlock(&g_routeMutex);
        
    //check interfaces
    //if any interfaces are now down, update entries through them to INFINITY
    for (int i=0;interfaces[i];i++)
    {
      int upstate, hashkey;
      upstate = (interfaces[i]->sock != -1);
      if (upstate == 0)
      {
        pthread_mutex_lock(&g_routeMutex);
        htable_iterate_begin(g_routeTable, hashkey, entry, routeentry_t) {
          if (entry->interface == interfaces[i] && entry->cost != ROUTE_INFINITY)
          {
            dbg(DBG_ROUTE, "route: Interface %s has gone down - route to %d lost.\n", 
                inet_ntoa_host(interfaces[i]->local_virt_ip), hashkey);
            entry->cost = ROUTE_INFINITY;
            triggerUpdate = 1;
          }
        } htable_iterate_end();
        pthread_mutex_unlock(&g_routeMutex);
      }
    }
        
    if (triggerUpdate)
      trigger_update();
    
    updateCounter++;
    sleep(1);
  }
  return NULL;
}

int route_init(interface_t **interfaces)
{
  pthread_t updateThread;
    
  if (g_routeTable != NULL) {
    dbg(DBG_ROUTE, "route_init: Routing already initialized.\n");
    return 0;
  }
  g_interfaces = interfaces;
    
  pthread_mutex_init(&g_routeMutex, NULL);
  pthread_mutex_init(&g_updateMutex, NULL);
  
  g_routeTable = (htable_t *)malloc(sizeof(htable_t));
  htable_init(g_routeTable, 10);

  //include our own interfaces before continuing (thanks to Dongbo for pointing this out)
  update_own_interfaces(interfaces);

  // Create a thread to handle updating
  if (pthread_create(&updateThread, NULL, update_thread, (void *)interfaces) != 0)
  {
    perror("pthread_create");
    return 0;
  }

  //register our handler
  net_register_handler(RIP_PROTOCOL_NUM, rip_handler);
  return 1;
}

int route_exit()
{    
  if (g_routeTable == NULL) {
    dbg(DBG_ROUTE, "route_exit: Routing already deinitialized.\n");
    return 0;
  }
    
  pthread_mutex_destroy(&g_routeMutex);
  pthread_mutex_destroy(&g_updateMutex);
    
  routeentry_t *entry;
  int hashkey;
  htable_iterate_begin(g_routeTable, hashkey, entry, routeentry_t) {
    free(entry);
  } htable_iterate_end();
    
  htable_destroy(g_routeTable);
  free(g_routeTable);
  g_routeTable = NULL;
  g_interfaces = NULL;
  return 1;
}

//we're not forwarding, so don't decrement the ttl here
int net_send_noroute(struct ip* packet, interface_t *interface)
{
  if (packet->ip_ttl == 0) {
    dbg(DBG_ROUTE, "net_send_noroute: ttl == 0, not sending packet.\n");
    return -EPROTO;
  }

  int length = packet->ip_len;
  ip_packet_hton(packet);    
  ip_calc_checksum(packet);
  int res = link_send(interface, (char *)packet, length);
  ip_packet_ntoh(packet);
  return res;
}

//locks the routing table mutex
interface_t *route_lookup(struct in_addr destAddress)
{
  pthread_mutex_lock(&g_routeMutex);    
  routeentry_t *entry =
    (routeentry_t *)htable_get(g_routeTable, destAddress.s_addr);    
  pthread_mutex_unlock(&g_routeMutex);
    
  if (entry == NULL || entry->cost == ROUTE_INFINITY) {
    return NULL;
  }
    
  return entry->interface;
}



