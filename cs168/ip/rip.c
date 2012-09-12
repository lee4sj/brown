#include "rip.h"
#include "network_layer.h"
#include "util/htable.h"
#include "util/list.h"
#include <stdio.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>


//******Global Variables
list_t *rip_table; //the forwarding table.
htable_t *addrtorip_map; //maps the network address to a pointer to rip (malloced)
pthread_mutex_t *rip_mutex; //mutex lock for accessing the forwarding table. (malloced)
list_t *interface_list; //list of interfaces

int update_counter; // when this reaches 5, the rip should broadcast its table.
int expired_counter; // when this reaches 12, the program should check for expired interfaces

#define MAX_NETWORK_SIZE 64
#define INFINITE 16
#define UPDATETIME 5
#define EXPIREDTIME 12
#define DEBUG_MODE 1




void *rip_loop(list_t *intrf_param) {

  rip_init(intrf_param);
  maintain_rip_table();
  end_rip();
}

/*
 * Assumes that rip_table is already initialized.
 */
void init_addrtorip_map(){
  
  addrtorip_map = (htable_t *) malloc(sizeof(htable_t));
  htable_init(addrtorip_map, MAX_NETWORK_SIZE);
  
  for(list_link_t* link = rip_table->l_next;
      link != rip_table; link = link->l_next){
    
    rip* cur_rip = list_item(link, rip, ripnode);
    htable_put(addrtorip_map, cur_rip->network_addr, cur_rip);
  }
}


/*
 * Assumes that interface is already initialized.
 */
void init_rip_table(){
  rip_table = (list_t *) malloc(sizeof(list_t));
  list_init(rip_table);

  for(list_link_t *link = interface_list->l_next;
      link != interface_list; link = link->l_next){
    
    interface* cur_intrf = list_item(link, interface, listnode);


    rip *cur_local_rip = (rip*)malloc(sizeof(rip));
    cur_local_rip->network_addr = cur_intrf->local_virt_ip.s_addr;
    cur_local_rip->cost = 0;
    cur_local_rip->next_hop = NULL;
    
    list_insert_tail(rip_table, &cur_local_rip->ripnode);

    
    rip *cur_remote_rip = (rip*)malloc(sizeof(rip));
    cur_remote_rip->network_addr = cur_intrf->remote_virt_ip.s_addr;
    cur_remote_rip->cost = INFINITE;
    cur_remote_rip->next_hop = cur_intrf;
    
    list_insert_tail(rip_table, &cur_remote_rip->ripnode);

  }


  print_routes_info();
}


void rip_init(list_t *interface_list_param){
  update_counter = 1;
  expired_counter = 1;


  rip_mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
  pthread_mutex_init(rip_mutex, NULL);

  interface_list = interface_list_param;

  init_rip_table();
  init_addrtorip_map();
}

void end_rip(){
  //free all rip
  for(list_link_t *link = rip_table->l_next;
      link != rip_table; link = link->l_next)
    free(list_item(link, rip, ripnode));
  
  free(rip_table);
  htable_destroy(addrtorip_map);  
}


struct entries{
  uint32_t cost;
  uint32_t address;
};



interface *get_interface(struct in_addr *virt_ip){

  //printf("get_interface called: %u\n", virt_ip->s_addr);

  if(virt_ip == NULL){
    //    printf("WRONG VIRT_IP\n");
    return NULL;
  }

  pthread_mutex_lock(rip_mutex);

  rip* cur_rip = htable_get(addrtorip_map, virt_ip->s_addr);
  

  if(cur_rip == NULL){// || cur_rip->cost == INFINITE){
    pthread_mutex_unlock(rip_mutex);
    return NULL;
  }

  if(cur_rip->cost == INFINITE){
    pthread_mutex_unlock(rip_mutex);
    printf("get_interface: INFINITE\n");
    return NULL;
  }
  
  interface *val = cur_rip->next_hop;
  pthread_mutex_unlock(rip_mutex);

  return val;
}



int send_request(interface* intrf){
  
  uint16_t message[2];
  message[0]= htons(1);
  message[1] = htons(0);

  char *packet = (char*)malloc(2*sizeof(uint16_t));
  memcpy(packet, message, 2*sizeof(uint32_t));
  
  //send_packet(packet, 2*sizeof(uint16_t), 200, intrf);
  send_packet(
        packet, 
        2*sizeof(uint16_t), 
        200, 
        intrf, 
        intrf->local_virt_ip.s_addr,
        intrf->remote_virt_ip.s_addr);

  free(packet);

  return 1;
}


int send_response(struct in_addr *src){
  
  uint16_t mssg_hdr[2];
  mssg_hdr[0] = htons(2);
  mssg_hdr[1] = 0;

  struct entries rip_ent[64]; 

  pthread_mutex_lock(rip_mutex);


  for(list_link_t *link = rip_table->l_next;
      link != rip_table;
      link = link->l_next){

    //compose the entries
    rip *cur_rip = list_item(link, rip, ripnode);
    rip_ent[mssg_hdr[1]].cost = htonl(cur_rip->cost);
    rip_ent[mssg_hdr[1]].address = htonl(cur_rip->network_addr);

    //if the cur_rip is not local entry.
    if(cur_rip->next_hop != NULL){
      //split horizon with poinson reverse
      //if the route is learned from the src, send poisoned reverse (infinity)
      interface *cur_intrf = cur_rip->next_hop;
      if(cur_rip->network_addr != src->s_addr && cur_intrf->remote_virt_ip.s_addr == src->s_addr)
	rip_ent[mssg_hdr[1]].cost = htonl(INFINITE);
    }

    mssg_hdr[1]++;    
  }
  pthread_mutex_unlock(rip_mutex);

  uint16_t num_entries = mssg_hdr[1];
  //  printf("%d", mssg_hdr[1]);

  mssg_hdr[1] = htons(mssg_hdr[1]);
  
  //  printf(": %d\n", mssg_hdr[1]);

  size_t pack_size = sizeof(uint16_t)*2 + num_entries*sizeof(struct entries);
  char* packet = malloc(pack_size);
  memcpy(packet, mssg_hdr, 4);
  memcpy(&packet[4], rip_ent, num_entries*sizeof(struct entries));
  

  interface *to = get_interface(src);
  struct in_addr temp = {htonl(src->s_addr)};

  if (to == NULL) {
    if(DEBUG_MODE)
      printf("warning: get_interface for address %s returned null\n", inet_ntoa(temp));
    return -1;
  }

  //if(send_packet(packet, pack_size, 200, get_interface(src)) < 0){
  if (send_packet(
           packet, 
           pack_size, 
           200, 
           to, 
           to->local_virt_ip.s_addr, 
           to->remote_virt_ip.s_addr) < 0) {
    free(packet);
    
    if(DEBUG_MODE)
      printf("%c[1;35mRIP: error: send_response to %s.%c[0m\n", 27, inet_ntoa(temp), 27);

    return -1;
  }

  free(packet);
  return 1;
}



int broadcast_rip(){

  update_counter = 1;

  for(list_link_t *link = interface_list->l_next;
      link != interface_list; link = link->l_next){
    
    interface* cur_intrf = list_item(link, interface, listnode);

    if(cur_intrf->enabled == 1)
      send_response(&cur_intrf->remote_virt_ip);
    
  }
  
  //  printf("**RIP: broadcasted rip table.\n");
  return 1;
}


interface* get_immediate_interface(struct in_addr *src){

  for(list_link_t *link = interface_list->l_next;
      link != interface_list; link = link->l_next){
    
    interface* cur_intrf = list_item(link, interface, listnode);

    if(cur_intrf->remote_virt_ip.s_addr == src->s_addr)
      return cur_intrf;
  }
  
  return NULL;
}


int update_rip_table(struct entries *ent_list, uint16_t num_ent, struct in_addr *src){

  if(DEBUG_MODE)
    printf("%c[1;35mRIP: update_rip_table.%c[0m\n", 27, 27);


  int trig_update = 0; //to see if we have to trigger the updates
  rip* src_rip = htable_get(addrtorip_map, src->s_addr);


  if(src_rip == NULL){
    //printf("no such neighbor exists!\n");
    return -1;
  }

  //time stamp the interface of the src with current time.
  struct timeval cur_time;
  gettimeofday(&cur_time, NULL);
  interface* src_intrf = get_immediate_interface(src);

  //if src interface is disabled, we fail at updating.
  if(src_intrf->enabled == 0)
    return -1;


  //update the src rip
  src_rip->cost = 1;
  src_rip->next_hop = src_intrf;
  src_rip->time_stamp = cur_time.tv_sec;

  pthread_mutex_lock(rip_mutex);//lock the rip_table
  
  /*  struct in_addr k1 = {htonl(src_rip->network_addr)};
      printf("routes update info from %s: \n", inet_ntoa(k1));*/

  //  printf("entering update loop: %d \n", num_ent);
  for(int i = 0; i < num_ent; i++){
    uint32_t cost = ntohl(ent_list[i].cost) + src_rip->cost;
    uint32_t address = ntohl(ent_list[i].address);

    /*    struct in_addr k = {ent_list[i].address};
	  printf("\t%s: %u\n", inet_ntoa(k), cost);*/
    
    rip* cur_rip = htable_get(addrtorip_map, address);

    //if address is not already in the list, simply add the entry.
    if(cur_rip == NULL){

      rip* new_rip = (rip*) malloc(sizeof(rip));
      new_rip->network_addr = address;
      new_rip->cost = cost;
      new_rip->next_hop =  src_intrf;
      list_insert_tail(rip_table, &new_rip->ripnode);
      htable_put(addrtorip_map, address, new_rip);
      cur_rip = new_rip;
    }

    //since the src entry is already updated, leave it.    
    //if cur_rip is not local destination, 
    else if (cur_rip->cost != 0 && cur_rip->network_addr != src->s_addr){
      
      //if the network address is currently reachable,
      if(cur_rip->cost < INFINITE){
	
	//if the updating cost is less then the current, update the rip.
	if(cost < cur_rip->cost){
	  cur_rip->cost = cost;
	  cur_rip->next_hop = src_rip->next_hop;
	  htable_put(addrtorip_map, address, cur_rip);
	}
	
	//if the route goes through the src and is now infinite, trigger the update and change the cost.
	else if(cost >= INFINITE &&  cur_rip->next_hop->remote_virt_ip.s_addr == src->s_addr){
	  trig_update = 1;
	  cur_rip->cost = INFINITE;
	}
	
	//otherwise, do nothing.
      }
      
      
      //otherwise, (if the network address is currently unreachable,
      else{
	
	//if the updating cost is reachable, update the entry
	if(cost < INFINITE){
	  
	  cur_rip->cost = cost;
	  cur_rip->next_hop = src_rip->next_hop;
	  
	}
      }
    }
    
    cur_rip->time_stamp = cur_time.tv_sec;
  }
  

  pthread_mutex_unlock(rip_mutex);//unlock the rip_table




  //  src_rip = htable_get(addrtorip_map, src->s_addr);

  /*  struct in_addr a = {htonl(src->s_addr)};
      printf("%s: %u\n", inet_ntoa(a), src_rip->cost);*/

  if(trig_update){ // if the update is triggerred, broadcast the updated list
    if(DEBUG_MODE)
      printf("%c[1;35mRIP: UPDATE_RIP_TABLE: triggered updates%c[0m\n", 27, 27);
    broadcast_rip();
  }

  return 1;
}


int handle_rip_packet(char *packet, size_t size, struct in_addr *src){

  
  if(DEBUG_MODE){
    struct in_addr nsrc;
    nsrc.s_addr = htonl(src->s_addr);
    printf("%s: received RIP packect.\n", inet_ntoa(nsrc));
  }
  

  if(size < 4)
    return -1;


  uint16_t command;
  memcpy(&command, packet, 2);
  command = ntohs(command);

  
  //if a request is received, send the current list.
  if( command == 1){
    pthread_mutex_lock(rip_mutex);
    rip* src_rip = htable_get(addrtorip_map, src->s_addr);
    src_rip->cost = 1;
    src_rip->next_hop = get_immediate_interface(src);
    pthread_mutex_unlock(rip_mutex);

    struct timeval cur_time;
    gettimeofday(&cur_time, NULL);

    src_rip->time_stamp = cur_time.tv_sec;

    /*    struct in_addr temp = {htonl(src_rip->network_addr)};
	  printf("%s: %u\n", inet_ntoa(temp), src_rip->cost);*/
    return send_response(src);
  }

  //if a response is received, update the rip table.
  else if(command == 2){

    uint16_t num_ent;
    memcpy(&num_ent, &packet[2], 2);
    num_ent = ntohs(num_ent);

    //    printf("%d", num_ent);

    //    printf("**RIP: update received!\n");
    return update_rip_table((struct entries *)&packet[4], num_ent, src);
  }
  
  //otherwise return -1 for the failure.
  else{
    return -1;
  }
}

int interface_up(int id){
  
  interface* cur_intrf;

  for(list_link_t *link = interface_list->l_next;
      link != interface_list;
      link = link->l_next){
    
    cur_intrf = list_item(link, interface, listnode);

    if(cur_intrf->id == id)
      break;

    cur_intrf = NULL;
  }

  if(cur_intrf == NULL)
    return -1;

  send_request(cur_intrf);

  if(DEBUG_MODE)
    printf("%c[1;35mRIP: interface %d is up.%c[0m\n", 27, id, 27);
  
  return 1;
}


int interface_down(int id){
  
  for(list_link_t *link = rip_table->l_next;
      link != rip_table;
      link = link->l_next){
      
    rip* cur_rip = list_item(link, rip, ripnode);
    
    if(cur_rip->cost != 0 && cur_rip->next_hop->id == id){
      cur_rip->cost = INFINITE;
    }
  }

  if(DEBUG_MODE)
    printf("%c[1;35mRIP: interface %d is down.%c[0m\n", 27, id, 27);
  
  return 1;
}
  
	
int check_expired(){
  
  struct timeval cur_time;
  gettimeofday(&cur_time, NULL);
  time_t cur_sec = cur_time.tv_sec;
  int trigger_updates = 0;
  
  

  for(list_link_t *link = rip_table->l_next;
      link != rip_table;
      link = link->l_next){
    
    rip* cur_rip = list_item(link, rip, ripnode);
    
    if((cur_rip->cost > 0 && cur_rip->cost < INFINITE) && (cur_sec - cur_rip->time_stamp) >= EXPIREDTIME){

      if(DEBUG_MODE)
	printf("%c[1;35mRIP: rip_table entry expired.%c[0m\n", 27, 27);

      trigger_updates = 1;
      cur_rip->cost = INFINITE;
    }
  }

  if(trigger_updates){
    if(DEBUG_MODE)
      printf("%c[1;35mRIP: TRIGGERED UPDATES%c[0m\n", 27, 27);
    broadcast_rip();
  }

  expired_counter = 1;
  //  printf("**RIP: expired check\n");
  return 1;
}


int maintain_rip_table(){
  
  struct timespec tv;
  tv.tv_sec = 1;
  tv.tv_nsec = 0;

  nanosleep(&tv, NULL);
  

  //1. send request to all the interfaces.
  for(list_link_t *link = interface_list->l_next;
      link != interface_list; link = link->l_next){
    
    interface* cur_intrf = list_item(link, interface, listnode);
    send_request(cur_intrf);
  }
  //  printf("sent request!!\n");


  //2. send the periodic updates to all of its interfaces every 5 seconds and
  //   check for the expired interfaces every 12 seconds.
  
  

  for(;;){

    //a. send periodic updates.
    if(update_counter >= UPDATETIME){
      if(DEBUG_MODE)
	printf("%c[1;35mRIP: SENT PERIODIC UPDATES%c[0m\n", 27, 27);
      broadcast_rip();
    }

    //b. check for the expired interfaces
    if(expired_counter >= EXPIREDTIME){
      if(DEBUG_MODE)
	printf("%c[1;35mRIP: PERIODIC EXPIRATION CHECK%c[0m\n", 27, 27);
      check_expired();
    }

    nanosleep(&tv, NULL);
    update_counter++;
    expired_counter++;
  }
}

int print_routes_info(){

  char routing_table[1300];
  bzero(routing_table, 1300);
  strcat(routing_table, "%c[1;35mRIP: routes information:");

  for(list_link_t *link = rip_table->l_next;
      link != rip_table; link = link->l_next){
    
    rip* cur_rip = list_item(link, rip, ripnode);
    
    if(cur_rip->cost < INFINITE){
      struct in_addr addr;
      addr.s_addr = htonl(cur_rip->network_addr);
      
      char rip_info[50];
      int r = sprintf(rip_info, "\n\t%s: %u", inet_ntoa(addr), cur_rip->cost);
      strcat(routing_table, rip_info);
    }
  }

  strcat(routing_table, "%c[0m\n");

  printf(routing_table, 27, 27);
}
