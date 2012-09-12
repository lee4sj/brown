#include <stdio.h>
#include <stdlib.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <inttypes.h>

#include "transport_layer.h"
#include "network_layer.h"
#include "rip.h"
#include "state_machine.h"
#include "util/dbg.h"
#include "util/ipsum.h"
#include "parselinks.h"
#include "tcp_thread.h"

// Global variables for interface list & mutex
list_t *interface_lst;
pthread_mutex_t *if_mutex;
pthread_t write_thread;


/*************************************************/
/* Stuff from old verison of node.c */

// From the K&R C book
void reverse(char s[])
 {
     int i, j;
     char c;
 
     for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
         c = s[i];
         s[i] = s[j];
         s[j] = c;
     }
 }

// Also from the K&R C book
void itoa(int n, char s[])
{
    int i, sign;

    if ((sign = n) < 0) 
        n = -n;         
    i = 0;
    do {      
        s[i++] = n % 10 + '0';  
    } while ((n /= 10) > 0);    
    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';
    reverse(s);
}



int int_strlen(int n) {
   int count = 0;

   do {
      count += 1;
   } while ((n /= 10) > 0);

   return count + 1;
}


/*
 * Reads the .lnx file and returns a list of interfaces.
 */
list_t *initialize_interfaces(char *lnx_path) {
   list_t *interface_lst;
   list_t *lnxfile_lst = parse_links(lnx_path);

   interface_lst = (list_t *)malloc(sizeof(list_t));
   list_init(interface_lst);

   if (lnxfile_lst == NULL) {
      fprintf(stderr, "error parsing .lnx file\n");
      exit(1);
   }

   list_link_t *link;
   int next_id = 0;
   for (link = lnxfile_lst->l_next; link != lnxfile_lst; link = link->l_next) {
      lnxline_t *current = list_item(link, lnxline_t, listnode);

      int sockfd;

      // Turn UDP port unsigned ints into strings
      int l = int_strlen(current->local_phys_port);
      char *lphys_str = (char *)malloc(sizeof(char) * l);
      itoa(current->local_phys_port, lphys_str);
      //printf("local phys addr: %s\n", current->local_phys_host);
      //printf("local phys port: %s\n", lphys_str);

      l = int_strlen(current->remote_phys_port);
      char *rphys_str = (char *)malloc(sizeof(char) * l);
      itoa(current->remote_phys_port, rphys_str);
      //printf("remote phys addr: %s\n", current->remote_phys_host);
      //printf("remote phys port: %s\n", rphys_str);

      // Get addrinfo struct for remote address + port 
      // (to store in interface struct as "to")
      int status;
      struct addrinfo r_hints;
      struct addrinfo *r_servinfo;

      memset(&r_hints, 0, sizeof(r_hints));
      r_hints.ai_family = AF_INET;
      r_hints.ai_socktype = SOCK_DGRAM;
      status = getaddrinfo(current->remote_phys_host,
            lphys_str,
            //NULL,
            &r_hints,
            &r_servinfo);      
      struct sockaddr_in *temp = (struct sockaddr_in *)r_servinfo->ai_addr;
      temp->sin_port = htons(current->remote_phys_port);
      
      if (status != 0) {
         fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
         exit(1);
      }
      
      // TODO: when can i call freeaddrinfo on r_servinfo?

      // Create socket
      struct addrinfo hints;
      struct addrinfo *servinfo, *p;  // will point to the results

      memset(&hints, 0, sizeof(hints)); // make sure the struct is empty
      hints.ai_family = AF_INET;     // IPV4
      hints.ai_socktype = SOCK_DGRAM; // UDP socket
      if ((status = getaddrinfo(current->local_phys_host, 
                  //lphys_str,
                  NULL,
                  &hints, 
                  &servinfo)) != 0) {
         fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
         exit(1);
      }

      for (p = servinfo; p != NULL; p = p->ai_next) {
      	temp = (struct sockaddr_in *)p->ai_addr;
      	temp->sin_port = htons(current->local_phys_port);

         sockfd = socket(p->ai_family,
               p->ai_socktype,
               p->ai_protocol);

         if (sockfd == -1) {
            continue;
         }

         if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("listener: bind");
            continue;
         }
 
         break;
      }

      if (p == NULL) {
         fprintf(stderr, "error binding socket\n");
         exit(1);
      }

      interface *current_if = (interface *)malloc(sizeof(interface));
      current_if->id = next_id;
      current_if->enabled = 1;
      current_if->socketfd = sockfd;
      current_if->to = r_servinfo->ai_addr;
      current_if->tolen = r_servinfo->ai_addrlen;
      current_if->local_virt_ip = current->local_virt_ip;
      current_if->remote_virt_ip = current->remote_virt_ip;
      list_insert_tail(interface_lst, &current_if->listnode);

      
      freeaddrinfo(servinfo); // free the linked-list
      next_id += 1;
   }

   return interface_lst;
}


void print_interface_info(list_t *interface_lst, pthread_mutex_t *m) {
   extern pthread_mutex_t *if_mutex;
   pthread_mutex_lock(if_mutex);
   for (list_link_t *link = interface_lst->l_next;
         link != interface_lst;
         link = link->l_next) {
      interface *current = list_item(link, interface, listnode);
      char locstr[INET6_ADDRSTRLEN], remstr[INET6_ADDRSTRLEN];

      uint32_t lvip = htonl(current->local_virt_ip.s_addr);
      uint32_t rvip = htonl(current->remote_virt_ip.s_addr);
      
      inet_ntop(AF_INET, &(lvip), locstr, sizeof(locstr));
      inet_ntop(AF_INET, &(rvip), remstr, sizeof(remstr));
      printf("interface %d: enabled = %d, local IP = %s, remote IP = %s\n",
            current->id, current->enabled, locstr, remstr);
   }
   pthread_mutex_unlock(if_mutex);
}


void handle_updown_command(int iface_num, 
      int new_status, 
      list_t *interface_lst, 
      pthread_mutex_t *m) {
   pthread_mutex_lock(m);
   for (list_link_t *link = interface_lst->l_next;
         link != interface_lst;
         link = link->l_next) {
      interface *current = list_item(link, interface, listnode);
      if (current->id == iface_num) {
         current->enabled = new_status;
	 
	 if(new_status == 0)
	   interface_down(iface_num);
	 else
	   interface_up(iface_num);
	 
         pthread_mutex_unlock(m);
	 
         return;
      }
   }
   pthread_mutex_unlock(m);
   fprintf(stderr, "error: invalid interface number\n"); 
}

list_t *get_interface_list() {
   return interface_lst;
}

pthread_mutex_t *get_interface_mutex() {
   return if_mutex;
}

/* iphdr in host byte order.  tcp header in network byte order*/
uint16_t tcp_sum(struct iphdr *ipheader, struct tcphdr *header, int len) {
   /*
   printf("in tcp_sum: \n");
   printf("ip header: \n");
   print_ip_header(ipheader);
   printf("tcp header: \n");
   print_tcp_header(header);
   printf("len = %u\n", len);
   */

   char pseudo_header[12 + len];
   memset(pseudo_header, 0, 12 + len);

   // The IP header fields had been translated to host byte order in
   // the network layer, before being passed to me.  The pseudo-header
   // needs them in network byte order, so I use these variables (without
   // modifying the ip header).
   uint32_t fixed_saddr = htonl(ipheader->saddr);
   uint32_t fixed_daddr = htonl(ipheader->daddr);
   memcpy(pseudo_header, &fixed_saddr, 4);
   memcpy(pseudo_header + 4, &fixed_daddr, 4);

   // Zero, protocol
   pseudo_header[8] = 0;
   pseudo_header[9] = TCP_PROTOCOLNUM;

   // TCP Length (in network byte order)
   uint16_t l = (uint16_t)len;
   l = htons(l);
   memcpy(pseudo_header + 10, &l, 2);

   // The TCP header + data
   memcpy(pseudo_header + 12, header, len);

   uint16_t rtn = ip_sum(pseudo_header, 12 + len);
   return rtn;
}


int handle_syn(struct tcphdr *header, 
      state_response *r, 
      connection *c, 
      uint32_t saddr,
      uint32_t daddr) {
   if (r->new_state == -1) {
      dbg(DBG_ERROR, "handle_tcp_packet: connection couldn't handle RCVSYN transition\n");
      return ENOTSUP;
   }

   // Create new waiting connection
   struct wt_conn *waiter = (struct wt_conn *)malloc(sizeof(struct wt_conn));
   waiter->d_addr = daddr;
   waiter->s_addr = saddr;
   waiter->s_port = header->source;
   waiter->d_port = header->dest;
   waiter->seq = header->seq;

   // Add to c's blocking queue
   // TODO: not sure if this needs to happen in all cases when receiving a SYN
   // could be only if you were previously in LISTEN state
   bqueue_enqueue(c->wt_conns, (void *)waiter);

   // SYN, ACK will be sent when thread waiting on wt_conns comes back

   return 0;
}


int handle_acksyn(struct tcphdr *header, state_response *r, connection *c) { 
   if (r->new_state == -1) {
      dbg(DBG_ERROR, "handle_tcp_packet: connection couldn't handle RCVACKSYN transition\n");
      return ENOTSUP;
   }

   pthread_mutex_lock(c->protector);
   c->state = ESTAB_ST;

   c->last_read = header->seq - 1;
   c->next_expected = c->last_read + 1;

   c->last_ack = header->ack_seq - 1;
   c->last_written = header->ack_seq - 1;
   c->last_sent = header->ack_seq - 1;
   c->window_size = header->window;

/*    if(DEBUG_MODE_TCP) */
/*      printf("last_ack: %u: %u\n", c->last_ack, header->ack_seq); */

   pthread_mutex_unlock(c->protector);

   pthread_cond_broadcast(c->conn_wait);

   pthread_create(&write_thread, 0, run_tcp_thread, c);

   return 0;
}


int handle_synack(struct tcphdr *header, state_response *r, connection *c, uint32_t daddr) { 
   if (r->new_state == -1) {
      dbg(DBG_ERROR, "handle_tcp_packet: connection couldn't handle RCVSYNACK transition\n");
      return ENOTSUP;
   }

   pthread_mutex_lock(c->protector);
   c->state = r->new_state;
   c->remote_seq = header->seq;
   
   c->last_read = header->seq - 1;
   c->next_expected = c->last_read + 1;

   c->last_ack = header->ack_seq - 1;
   c->last_written = header->ack_seq - 1;
   c->last_sent = header->ack_seq - 1;
   c->window_size = header->window;

/*    if(DEBUG_MODE_TCP) */
/*      printf("synack, %u: %u\n", c->last_ack, header->ack_seq); */
   

   pthread_mutex_unlock(c->protector);

   pthread_cond_broadcast(c->conn_wait);

   pthread_create(&write_thread, 0, run_tcp_thread, c);

   return 0;
}


int handle_fin(struct tcphdr *header, state_response *r, connection *c, int had_data) {
   if (r->new_state == -1) {
      //dbg(DBG_ERROR, "handle_fin: connection couldn't handle RCVFIN transition\n");
      return -ENOTSUP;
   }

   pthread_mutex_lock(c->protector);

   //printf("got FIN: SEQ = %u, ACK_SEQ = %u\n", header->seq, header->ack_seq);
   /*
   printf("header SEQ: %u\n", header->seq);
   printf("header ACK: %u\n", header->ack_seq);
   printf("c->local_seq: %u\n", c->local_seq);
   printf("c->remote_seq: %u\n", c->remote_seq);
   printf("c->last_ack: %u\n", c->last_ack);
   printf("c->last_written: %u\n", c->last_written);
   printf("c->last_sent: %u\n", c->last_sent);
   printf("c->last_read: %u\n", c->last_read);
   printf("c->next_expected: %u\n", c->next_expected);
   printf("\n");
   */

   c->state = r->new_state;
   uint32_t ack_seq = c->next_expected;
   //if (!had_data) ack_seq += 1;
   
   printf("handle fin sending ACK: SEQ = %u, ACK_SEQ = %u\n", c->last_ack + 1, ack_seq + 1);
   tcp_send_packet(c, c->last_ack+1, ack_seq + 1, 0, 0, 1, NULL, 0);

   int latest_state = c->state;
   pthread_mutex_unlock(c->protector);

   pthread_cond_broadcast(c->conn_wait); // if any threads waiting in v_read, wake them

   // If the new state is TIMEWAIT, then create a thread to move this conneciton to closed
   // after the delay period.
   if (latest_state == TIMEWT_ST) {
      set_timeout(c);
   }

   return 0;
}


int handle_ackfin(struct tcphdr *header, state_response *r, connection *c) {
   if (r->new_state == -1) {
      dbg(DBG_ERROR, "handle_ackfin: connection couldn't handle RCVACKFIN transition\n");
      return -ENOTSUP;
   }
   
   pthread_mutex_lock(c->protector);
   if(header->ack){
     uint32_t ack = header->ack_seq;
     if(less_than(c->last_ack, ack - 1)) {
       c->last_ack = ack - 1;
     } 
   }

   c->state = r->new_state;
   pthread_mutex_unlock(c->protector);

   if (r->new_state == CLOSED_ST) {
      // were in LASTACK, now should clean up and delete the connection
      htable_t *sockets = get_sockets();
      pthread_mutex_t *m = get_connections_mutex();

      pthread_mutex_lock(m);
      htable_remove(sockets, c->sockfd);
      pthread_mutex_unlock(m);

      free_connection(c);
   }

   if (r->new_state == TIMEWT_ST) {
      set_timeout(c);
   }

   return 0;
}

int less_than(uint32_t a, uint32_t b){
  uint32_t diff = b - a;
  if(0 < b-a && b-a < 0x80000000) {
    return 1;
  }
  return 0;
}


/* Given the 4 tuple that identifies a conneciton, return the associated connection,
 * or NULL if it doesn't exist */
connection *find_target(struct iphdr *ipheader, struct tcphdr *header) {
   //printf("in find target\n");

   uint32_t daddr = ipheader->daddr;
   uint32_t saddr = ipheader->saddr;
   uint16_t sport = header->source;
   uint16_t dport = header->dest;

   char sip[INET_ADDRSTRLEN], dip[INET_ADDRSTRLEN];
   inet_ntop(AF_INET, &saddr, sip, INET_ADDRSTRLEN);
   inet_ntop(AF_INET, &daddr, dip, INET_ADDRSTRLEN);
   /*
   printf("looking for daddr %s, dport %u, saddr %s, sport %u\n\n",
         dip, dport, sip, sport);
         */

   htable_t *connections = get_sockets();
   pthread_mutex_t *m = get_connections_mutex();

   // First try to find a fully matching connection
   int sockfd;
   connection *conn;
   pthread_mutex_lock(m);
   htable_iterate_begin(connections, sockfd, conn, connection) {
      pthread_mutex_lock(conn->protector);
      char cur_sip[INET_ADDRSTRLEN], cur_dip[INET_ADDRSTRLEN];
      inet_ntop(AF_INET, &(conn->local_addr), cur_sip, INET_ADDRSTRLEN);
      inet_ntop(AF_INET, &(conn->remote_addr), cur_dip, INET_ADDRSTRLEN);
      /*
      printf("current: localaddr = %s, localport = %u, remoteaddr = %s, remoteport = %u\n",
            cur_sip, conn->local_port, cur_dip, conn->remote_port);
            */
      if(daddr == conn->local_addr &&
            dport == conn->local_port &&
            saddr == conn->remote_addr &&
            sport == conn->remote_port) {
         pthread_mutex_unlock(conn->protector);
         pthread_mutex_unlock(m);
         //printf("match\n");
         return conn;
      }
      pthread_mutex_unlock(conn->protector);
      //printf("no match\n");
   } htable_iterate_end();
   pthread_mutex_unlock(m);

   // If nothing matches, the packet could be a SYN going to
   // a listening socket (which has no source or destination address
   // yet)
   // TODO: could also be going to a SYNSENT socket (simultaneous open)
   if (header->syn) {
      conn = NULL;
      sockfd = 0;

      pthread_mutex_lock(m);
      htable_iterate_begin(connections, sockfd, conn, connection) {
         pthread_mutex_lock(conn->protector);
         if (conn->state == LISTEN_ST && conn->local_port == dport) {
            pthread_mutex_unlock(conn->protector);
            pthread_mutex_unlock(m);
            return conn;
         }
         pthread_mutex_unlock(conn->protector);
      } htable_iterate_end();
      pthread_mutex_unlock(m);
   }

   return NULL;
}


int read_from_buffer(char *buffer, uint32_t offset, char *to, uint16_t size) {
   uint16_t t_offset = offset % BUFFER_SIZE;

   if (((t_offset + size) % BUFFER_SIZE) < t_offset) {
      // wrap around
     int buf_index = t_offset, to_index = 0;
     for (; buf_index < BUFFER_SIZE; buf_index++) {
        to[to_index] = buffer[buf_index];
        to_index++;
     }

     buf_index = 0;

     for(; to_index < size; to_index++) {
        //buffer[buf_index] = source[to_index];
        to[to_index] = buffer[buf_index];
        buf_index++;
     }

   } else {
      //memcpy(&buffer[t_offset], source, size);
      memcpy(to, &buffer[t_offset], size);
   }
}


int write_to_buffer(char *buffer, uint32_t offset, char *source, uint16_t size){
  uint16_t t_offset = offset % BUFFER_SIZE;
  //printf("t_offset: %u, size: %u, added: %u\n", t_offset, size, t_offset + size);

  //if data reaches the end
  if(size != 0 && ((t_offset + size) % BUFFER_SIZE) <= t_offset){

     //int space_left = BUFFER_SIZE - t_offset;
     int buf_index = t_offset, src_index = 0;
     for (; buf_index < BUFFER_SIZE; buf_index++) {
        buffer[buf_index] = source[src_index];
        src_index++;
     }

     buf_index = 0;

     for(; src_index < size; src_index++) {
        buffer[buf_index] = source[src_index];
        buf_index++;
     }
     
     /*
    memcpy(&buffer[t_offset % BUFFER_SIZE], source, BUFFER_SIZE - t_offset);
    
    size = size - (BUFFER_SIZE - t_offset);

    memcpy(buffer, &source[BUFFER_SIZE - t_offset], size);
     */
  }
  else
    memcpy(&buffer[t_offset], source, size);

  
}

int receive_data(struct iphdr *ipheader, struct tcphdr *header, connection *target, char *packet){

   //***********************************************************
   //TODO: write the data to the read_buffer.
   //TODO: need to find where this goes in.
   //***********************************************************

   pthread_mutex_lock(target->protector);

   uint32_t seq = header->seq;
   uint32_t last_read = target->last_read;
   uint32_t next_expected = target->next_expected;
  
   char* buffer = target->recv_buffer;
   char* data = &packet[sizeof(struct iphdr) + sizeof(struct tcphdr)];
   uint16_t data_size = ipheader->tot_len - ipheader->ihl*4 - header->doff*4;
   //uint16_t data_size = ipheader->tot_len - sizeof(struct iphdr) - sizeof(struct tcphdr);
   uint32_t recv_window_size = BUFFER_SIZE - (next_expected - 1 - last_read);


   
   dbg(DBG_ERROR, "seq: %u, DATA: %u\n", seq, data_size);
   
   
   //*****
   if(data_size <= 0) {
      if (header->fin) {
         target->last_expected = header->seq;
      }
     pthread_mutex_unlock(target->protector);
     return 0;
   }


   //***check for the sequence number
   if(next_expected == seq){

     if(data_size > recv_window_size){
       data_size = recv_window_size;
     }

     write_to_buffer(buffer, next_expected, data, data_size);

     buffer_list* curr = target->recv_root->next;

     next_expected += data_size;
     
     while(curr != NULL && less_than(curr->seq-1,  next_expected)){
       
       if(less_than(next_expected, curr->seq+curr->size))
	 next_expected = curr->seq + curr->size;
       
       target->recv_root->next = curr->next;
       free(curr);
       curr = target->recv_root->next;

     }

     target->next_expected = next_expected;
     
   } else if(less_than(next_expected, seq) && less_than(seq, next_expected + recv_window_size) ) {
     // the seq is within the scope of buffer.
     //fix data_size accordingly

     printf("in here: seq = %u, data_size = %u\n", seq, data_size);

     
     if(!less_than(seq+data_size, next_expected+recv_window_size)){
       data_size = next_expected + recv_window_size - seq;
     }
     printf("seq2 = %u, size2 = %u\n", seq, data_size);

     write_to_buffer(buffer, seq, data, data_size);

     //append this to buffer_list
     buffer_list *prev = target->recv_root;
     buffer_list *curr = prev->next;
     int end = 0;
     while(end == 0){

       if(curr == NULL){
	 struct buffer_list* new_node = (buffer_list *) malloc(sizeof(struct buffer_list));
	 new_node->seq = seq;
	 new_node->size = data_size;
	 new_node->next = curr;
	 prev->next = new_node;
	 end = 1;
       }
       
       else if(less_than(seq-1, curr->seq)){
	 if(less_than(seq+data_size - 1, curr->seq)){
	   struct buffer_list* new_node = (buffer_list *) malloc(sizeof(struct buffer_list));
	   new_node->seq = seq;
	   new_node->size = data_size;
	   new_node->next = curr;
	   prev->next = new_node;
	 }

	 else if(less_than(seq+data_size - 1, curr->seq + curr->size)){
	   printf("******IN********: curr->seq = %u, seq = %u, data_size = %u, ", curr->seq, seq, curr->size);
	   curr->size = curr->size + (curr->seq - seq);
	   printf(" %u,\n ", curr->size);
	   curr->seq = seq;
	 }

	 else{
	   printf("----------------------IN********\n");
	   curr->size = data_size;
	   curr->seq = seq;
	 }

	 end = 1;
       }
       
       /*       else if(less_than(curr->seq, seq) && less_than(curr->seq + curr->size, seq)){
	 if(!less_than(curr->seq + curr->size, seq + data_size))
	   curr->size = seq+data_size - curr->seq;
	 
	 end = 1;
	 }*/
       else{
	 prev = curr;
	 curr = curr->next;
       }
     }



   }

   // send_ack();
   /*   printf("receive data sending last_ack = %u, next_expected = %u\n", 
	target->last_ack + 1, target->next_expected);*/
   tcp_send_packet(target, target->last_ack + 1, target->next_expected, 0, 0, 1, NULL, 0);
   
   
   if (header->fin) {
      //printf("set last_expected = %u\n", data_size + header->seq);
      target->last_expected = data_size + header->seq;
   }


   

/*    if(DEBUG_MODE_TCP) */
   dbg(DBG_ERROR, "received: %u: %u: %u\n",target->next_expected,  seq, data_size);

   pthread_mutex_unlock(target->protector);

   pthread_cond_broadcast(target->conn_wait); // unblock waiting readers (if any)

   return 1;
}

int update_send_buffer_param(connection *conn, uint32_t ack, uint16_t window_size){


  /*****************************************************************/
   //TODO: update the write_buffer param.
   /*****************************************************************/
  pthread_mutex_lock(conn->protector);


  dbg(DBG_ERROR, "updating: ack = %u, our_ack = %u\n", ack , conn->last_ack);

  if(conn->last_ack == ack - 1)
    conn->window_size = window_size;
  else if(less_than(conn->last_ack, ack - 1)) {
    conn->last_ack = ack - 1;
    conn->window_size = window_size;
    retran_node *curr = conn->send_root->next;

    while(curr != conn->send_root){
      if(curr->seq < ack){
	if(curr->seq + curr->size <= ack){
	 
	  if(curr->add_to_srtt){
	    struct timespec rtt;
	    clock_gettime(CLOCK_REALTIME, &rtt);
	    sub_time( &rtt, &curr->sent_time.tv_sec);
	    calc_SRTT(&conn->SRTT, &rtt);
	  }

	  /*
	  dbg(DBG_ERROR, "removing: %u, %u\n", curr->seq, curr->size);
	  dbg(DBG_ERROR, "%x, %x\n", conn->send_root, curr);
	  */
	  
	  retran_node *temp = curr;
	  curr = curr->next;
	  remove_node(conn->send_root, temp);
	  free(temp);
	  
	}
	else{
	  curr->size = curr->size - (ack - curr->seq);
	  curr->seq = ack;
	  retran_node *temp = curr;
	  curr = curr->next;
	  /*	  
	  while(temp != conn->send_root && temp->seq + temp->size == curr->seq && temp->size + curr->size <= MAX_SEG_SIZE){
	    curr->size = temp->size + curr->size;
	    curr->seq = temp->seq;
	    remove_node(conn->send_root, temp);
	    free(temp);
	    temp = curr;
	    curr = curr->next;
	  }
	  */
	}
      }else
	curr = curr->next;

    }
    
/*     clock_gettime(CLOCK_REALTIME, &conn->keep_alive);     */
/*     pthread_mutex_unlock(conn->protector); */
       
/*     if (wrote_all_bytes(conn)) { */
/*        close_write(conn); */
/*     } */
      

/*     pthread_cond_broadcast(conn->conn_wait_write); */
/*     pthread_cond_broadcast(conn->conn_wait_write2); */

/*     return 1 */

  }

/*   if(DEBUG_MODE_TCP) */
/*       printf("update2, ack: %u\n", conn->last_ack); */


  if(wrote_all_bytes(conn)){
    clock_gettime(CLOCK_REALTIME, &conn->keep_alive);
    pthread_mutex_unlock(conn->protector);
    close_write(conn);
  }else{
    clock_gettime(CLOCK_REALTIME, &conn->keep_alive);
    pthread_mutex_unlock(conn->protector);
  }
  
  

  pthread_cond_broadcast(conn->conn_wait_write);
  pthread_cond_broadcast(conn->conn_wait_write2);
  return 0;
}


void handle_tcp_packet(char *packet) {


  
  
   // packet is the whole packet, IP header included
   // IP header fields already translated to host byte order
   // before they got here
   struct iphdr *ipheader = (struct iphdr *)packet;
   int total_length = ipheader->tot_len;

   //char ip[INET_ADDRSTRLEN];
   //inet_ntop(AF_INET, &(ipheader->saddr), ip, INET_ADDRSTRLEN);
   //printf("from %s\n", ip);

   // this is the data of the IP packet, it hasn't been touched
   // all fields are in network byte order
   struct tcphdr *header = (struct tcphdr *)(packet + (sizeof(struct iphdr)));
   int tcp_length = total_length - sizeof(struct iphdr);


   
   //uint16_t checksum = tcp_sum(ipheader, header, sizeof(struct tcphdr));
   uint16_t checksum = tcp_sum(ipheader, header, tcp_length);
   if (checksum != 0) {
      printf("tcp checksum not zero, discarding\n");
      return;
   }
   
   // TCP header fields: network to host byte order
   header->source = ntohs(header->source);
   header->dest = ntohs(header->dest);
   header->seq = ntohl(header->seq);
   header->ack_seq = ntohl(header->ack_seq);
   header->window = ntohs(header->window);
   header->check = ntohs(header->check);
   header->urg_ptr = ntohs(header->urg_ptr);


   
   //printf("got packet: SEQ = %u, ACK_SEQ = %u\n", header->seq, header->ack_seq);
   //print_tcp_header(header);
   
   // Find connection with local port == destination port of packet
   connection *target = find_target(ipheader, header);



   if (target == NULL) {
      //dbg(DBG_ERROR, "handle_tcp_packet: connection not found\n");
      return;
   }

   // TODO: a lot more goes here
   state_response *r;
   pthread_mutex_lock(target->protector);

   //printf("recved a packet: ack:%u, our ack:%u\n", header->ack_seq, target->last_ack);
   
   int target_state = target->state;
   pthread_mutex_unlock(target->protector);

   if (header->fin) {
      int had_data = receive_data(ipheader, header, target, packet);
      r = next_state(target_state, RCVFIN_TR);
      handle_fin(header, r, target, had_data);
      if (!had_data) {
         pthread_cond_broadcast(target->conn_wait); // unblock waiting readers (if any)
      }
   } else if (header->syn) {
      if (header->ack) {
         r = next_state(target_state, RCVSYNACK_TR);
         handle_synack(header, r, target, ipheader->daddr);
      } else {
         r = next_state(target_state, RCVSYN_TR);
         handle_syn(header, r, target, ipheader->saddr, ipheader->daddr);
      }
   } else if (header->ack) {
      if (target_state == SYNRCVD_ST) { 
         r = next_state(target_state, RCVACKSYN_TR);
         handle_acksyn(header, r, target);
      } else if (target_state == FINWT1_ST || target_state == LASTACK_ST ||
            target_state == CLOSING_ST) {
         // it's an ACK of FIN
         r = next_state(target_state, RCVACKFIN_TR);
         handle_ackfin(header, r, target);
      } else {

         if (target->state == ESTAB_ST ||
            target->state == FINWT1_ST ||
            target->state == FINWT2_ST ||
            target->state == TIMEWT_ST ||
            target->state == CLOSEWT_ST ||
            target->state == LASTACK_ST) {
	   //            printf("before receive_data: \n");
           // printf("last_read = %u, next_expected = %u\n", 
	   //                  target->last_read, target->next_expected);
            receive_data(ipheader, header, target, packet);
	    update_send_buffer_param(target, header->ack_seq, header->window);
	    // printf("after receive_data: \n");
            //printf("last_read = %u, next_expected = %u\n", 
            //      target->last_read, target->next_expected);
            //printf("\n");
      /*
      printf("sending ack: SEQ = %u, ACK_SEQ = %u\n", 
            target->last_ack,
            target->next_expected);
      printf("\n");
      */
         }
      }

   } 

}


void transport_layer_init(char *lnxfile) {
  interface_lst = initialize_interfaces(lnxfile);
  //pthread_mutex_t if_mutex;
  if_mutex = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
  pthread_mutex_init(if_mutex, NULL);

  // Give reference to interface list and its mutex to link layer
  link_layer_init(interface_lst, if_mutex);

  // start forwarding thread
  pthread_cond_t *nlayer_ready = (pthread_cond_t *)malloc(sizeof(pthread_cond_t));
  pthread_cond_init(nlayer_ready, NULL);
  pthread_mutex_t ready_mutex;
  pthread_mutex_init(&ready_mutex, NULL);
  int ready_flag = 0;
  struct timespec deadline;

  nlayer_args a;
  a.interface_lst = interface_lst;
  a.if_mutex = if_mutex;
  a.ready = nlayer_ready;
  a.ready_mutex = &ready_mutex;
  a.ready_flag = &ready_flag;
  void *arg = (void *)(&a);

  pthread_t nl_thread;
  int result = pthread_create(&nl_thread, NULL, network_layer_init, arg);
  if (result != 0) {
    fprintf(stderr, "pthread_create error: %s\n", gai_strerror(result));
    exit(1);
  }
  // Wait for network layer to be ready before proceeding
  while (!ready_flag) {
      clock_gettime(CLOCK_REALTIME, &deadline);
      deadline.tv_sec += 1;
      pthread_cond_timedwait(nlayer_ready, &ready_mutex, &deadline);
  }
  pthread_cond_destroy(nlayer_ready);


  // Start RIP thread
  pthread_cond_t *rip_ready = (pthread_cond_t *)malloc(sizeof(pthread_cond_t));
  pthread_cond_init(rip_ready, NULL);
  ready_flag = 0;

  rip_args r;
  r.interface_lst = interface_lst;
  r.ready = rip_ready;
  r.ready_mutex = &ready_mutex;
  r.ready_flag = &ready_flag;

  pthread_t routing_thread;
  //arg = (void *)interface_lst;
  arg = (void *)(&r);
  result = pthread_create(&routing_thread, NULL, rip_loop, arg);
  if (result != 0) {
    fprintf(stderr, "pthread_create error: %s\n", gai_strerror(result));
    exit(1);
  }

  // Wait for RIP ready signal
  while (!ready_flag) {
      clock_gettime(CLOCK_REALTIME, &deadline);
      deadline.tv_sec += 1;
      pthread_cond_timedwait(rip_ready, &ready_mutex, &deadline);
  }
  pthread_cond_destroy(rip_ready);

  pthread_mutex_destroy(&ready_mutex);

  state_machine_init();
  socket_layer_init();

  handler_t tcp_handler = &handle_tcp_packet;
  net_register_handler(TCP_PROTOCOLNUM, tcp_handler);
}


void print_tcp_header(struct tcphdr *h) {
   printf("TCP Header: \n");
   //printf("size = %u\n", sizeof(struct tcphdr));
   printf("    source: %u\n", h->source);
   printf("    dest: %u\n", h->dest);
   printf("    seq: %u\n", h->seq);
   printf("    ack_seq: %u\n", h->ack_seq);
   printf("    res1: %u\n", h->res1);
   printf("    doff: %u\n", h->doff);
   printf("    fin: %u\n", h->fin);
   printf("    syn: %u\n", h->syn);
   printf("    rst: %u\n", h->rst);
   printf("    psh: %u\n", h->psh);
   printf("    ack: %u\n", h->ack);
   printf("    urg: %u\n", h->urg);
   printf("    res2: %u\n", h->res2);
   printf("    window: %u\n", h->window);
   printf("    check: %u\n", h->check);
   printf("    urg_ptr: %u\n", h->urg_ptr); /// 
}


/* expects addr in network byte order, all else in host byte order */
int tcp_send_packet(connection *c, 
      uint32_t seq,
      uint32_t ack_seq,
      int fin,
      int syn,
      int ack,
      char *data,
      unsigned int data_len) {

  char *packet = (char *)malloc(sizeof(struct tcphdr) + data_len);
  struct tcphdr *header = (struct tcphdr *)packet;
   /*
   printf("tcp_send_packet\n");
   printf("\n");
   char sip[INET_ADDRSTRLEN];
   inet_ntop(AF_INET, &(c->local_addr), sip, INET_ADDRSTRLEN);
   printf("saddr = %s\n", sip);
   char rip[INET_ADDRSTRLEN];
   inet_ntop(AF_INET, &(c->remote_addr), rip, INET_ADDRSTRLEN);
   printf("daddr = %s\n", rip);
   printf("fin = %u\n", fin);
   printf("syn = %u\n", syn);
   printf("ack = %u\n", ack);
   */

   //interface *i = get_interface(&addr);
   struct in_addr to;
   to.s_addr = c->remote_addr;
   interface *i = get_interface(&to);

   if (i == NULL) {
      dbg(DBG_ERROR, "tcp_send_packet: no route to c remote address\n");
      return -EFAULT;
   }

   if (c->local_addr == 0) c->local_addr = i->local_virt_ip.s_addr;

   header->source = c->local_port;
   header->dest = c->remote_port;
   header->seq = seq;
   header->ack_seq = ack_seq;
   header->res1 = 0;
   header->doff = 5;
   header->fin = fin;
   header->syn = syn;
   header->rst = 0;
   header->psh = 0;
   header->ack = ack;
   header->urg = 0;
   header->res2 = 0;

   uint32_t w = BUFFER_SIZE - (c->next_expected - 1 - c->last_read);
   if (w >= 0xFFFF) w = 0xFFFF;
   header->window = w;

   //header->window = 0xFFFF; // TODO: sliding window
   header->check = 0;
   header->urg_ptr = 0;

   /*
   printf("tcp_send_packet: ");
   print_tcp_header(header);
   */

   // To network byte order
   header->source = htons(header->source);
   header->dest = htons(header->dest);
   header->seq = htonl(header->seq);
   header->ack_seq = htonl(header->ack_seq);
   header->window = htons(header->window);
   header->urg_ptr = htons(header->urg_ptr);

   if(data != NULL)
     memcpy(&packet[sizeof(struct tcphdr)], data, data_len);
   
   //TODO: Need to make sure the checksum is right!
   // Compute and store checksum
   /* iphdr in host byte order.  tcp header in network byte order*/
   struct iphdr ipheader;
   //ipheader.saddr = htonl(c->local_addr);
   ipheader.saddr = c->local_addr;
   //ipheader.daddr = htonl(c->remote_addr);
   ipheader.daddr = c->remote_addr;
   uint16_t checksum = tcp_sum(&ipheader, packet, sizeof(struct tcphdr) + data_len);
   header->check = checksum;

   /*
   char *packet;
   if (data == NULL) {
      packet = (char *)header;
   } else {
      packet = (char *)malloc(sizeof(struct tcphdr) + data_len);
      memcpy(packet + sizeof(struct tcphdr), data, data_len);
TODO: finish implementing
   }
   */

   int rtn =
      send_packet(packet, 
            sizeof(struct tcphdr) + data_len, 
            TCP_PROTOCOLNUM, 
            i, 
            c->local_addr,
            c->remote_addr);

   free(packet);
   return rtn;
}


void *do_timeout(void *arg) {
   struct connection *c = (struct connection *)arg;
   pthread_mutex_lock(c->protector);
   if (c->state == CLOSED_ST) {
      pthread_mutex_unlock(c->protector);
      return;
   }

   if (c->state != TIMEWT_ST) {
      dbg(DBG_ERROR, "WARNING: timeout set on a connection with state = %d\n", c->state);
   }

   c->n_timeouts += 1;
   pthread_mutex_unlock(c->protector);

   printf("starting timeout...\n");
   sleep(5);

   int sockfd = -1;
   pthread_mutex_lock(c->protector);
   c->n_timeouts -= 1;
   if (c->n_timeouts == 0) {
      printf("timed out, closing socket\n");
      c->state = CLOSED_ST;
      sockfd = c->sockfd;
   }
   pthread_mutex_unlock(c->protector);

   if (sockfd >= 0) {
      htable_t *sockets = get_sockets();
      pthread_mutex_t *m = get_connections_mutex();

      pthread_mutex_lock(m);
      htable_remove(sockets, sockfd);
      pthread_mutex_unlock(m);

      free_connection(c);
   }

   pthread_exit(NULL);
}


/*
 * Create and start a thread that waits 5 seconds, then changes c's state from TIMEWAIT
 * to CLOSED.
 */
void set_timeout(struct connection *c) {
   pthread_t timeout_thread;
   int result = pthread_create(&timeout_thread, NULL, do_timeout, (void *)c);
   pthread_detach(timeout_thread);
}


