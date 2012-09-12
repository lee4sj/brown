#include <stdlib.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <time.h>
#include <errno.h>
#include "transport_layer.h"
#include "socket_api.h"
#include "state_machine.h"
#include "util/htable.h"
#include "util/dbg.h"
#include "util/bqueue.h"


// Global variable to hold connections
// TODO: need mutexes?
htable_t connections;
pthread_mutex_t c_mutex;
int table_size;


void socket_layer_init() {
   htable_init(&connections, MAX_CONNECTIONS);
   pthread_mutex_init(&c_mutex, NULL);
   table_size = 0;
}


uint16_t random_unused_port() {
   struct timespec current;
   clock_gettime(CLOCK_REALTIME, &current);
   uint16_t rtn = (uint16_t)(current.tv_nsec % 100000);

   int sockfd;
   connection *conn;
   int ok = 1;
   pthread_mutex_lock(&c_mutex);
   htable_iterate_begin(&connections, sockfd, conn, connection) {
      if (conn->local_port == rtn) {
         ok = 0;
      }
   } htable_iterate_end();
   pthread_mutex_unlock(&c_mutex);

   if (ok)
      return rtn;
   return random_unused_port();
}


uint32_t random_initial_seq() {
   struct timespec current;
   clock_gettime(CLOCK_REALTIME, &current);

   uint32_t rtn = (uint32_t)(current.tv_nsec % 100000);
   return rtn;
}


/* returns pointer to sockets hash table */
htable_t *get_sockets() {
   return &connections;
}


/* returns pointer to connections table mutex */
pthread_mutex_t *get_connections_mutex() {
   return &c_mutex;
}


/*
 * Returns a new connection, or NULL if the max have already been created
 */
connection *new_connection() {
   if (table_size == MAX_CONNECTIONS) {
      return NULL;
   }

   connection *new_conn = (connection *)malloc(sizeof(connection));
   new_conn->sockfd = table_size;
   new_conn->state = CLOSED_ST;
   new_conn->can_read = 1;
   new_conn->is_closed = 0;
   new_conn->n_timeouts = 0;
   new_conn->done_writing = 0;
   new_conn->thread_running = 1;
   new_conn->last_expected = 0;
   new_conn->local_addr = 0;
   new_conn->remote_addr = 0;
   new_conn->local_port = 0;
   new_conn->remote_port = 0;
   new_conn->remote_seq = 0;

   new_conn->recv_root = (buffer_list *)malloc(sizeof(buffer_list));
   new_conn->recv_root->next = NULL;

   new_conn->send_root = (retran_node *)malloc(sizeof(retran_node));
   new_conn->send_root->next = new_conn->send_root;
   new_conn->send_root->prev = new_conn->send_root;

   new_conn->window_size = BUFFER_SIZE; 

   new_conn->conn_wait = (pthread_cond_t *)malloc(sizeof(pthread_cond_t));
   pthread_cond_init(new_conn->conn_wait, NULL);
   new_conn->cw_mut = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
   pthread_mutex_init(new_conn->cw_mut, NULL);

   new_conn->conn_wait_write = (pthread_cond_t *)malloc(sizeof(pthread_cond_t));
   pthread_cond_init(new_conn->conn_wait_write, NULL);
   new_conn->cw_mut_write = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
   pthread_mutex_init(new_conn->cw_mut_write, NULL);

   
   new_conn->conn_wait_write2 = (pthread_cond_t *)malloc(sizeof(pthread_cond_t));
   pthread_cond_init(new_conn->conn_wait_write2, NULL);
   new_conn->cw_mut_write2 = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
   pthread_mutex_init(new_conn->cw_mut_write2, NULL);

   
   // Blocking queue for use with v_accept()
   new_conn->wt_conns = (bqueue_t *)malloc(sizeof(bqueue_t));
   bqueue_init(new_conn->wt_conns);

   new_conn->protector = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
   pthread_mutex_init(new_conn->protector, NULL);

   new_conn->recv_buffer = (char *)malloc(BUFFER_SIZE);
   new_conn->send_buffer = (char *)malloc(BUFFER_SIZE);

   table_size += 1;
   return new_conn;
}


/* returns a new, unbound socket. on failure, returns a negative value */
int v_socket() {
   connection *new_conn = new_connection();

   if (new_conn == NULL) {
      dbg(DBG_ERROR, "v_socket: max connections already created\n");
      return -ENFILE;
   }

   pthread_mutex_lock(&c_mutex);
   htable_put(&connections, new_conn->sockfd, new_conn);
   pthread_mutex_unlock(&c_mutex);

   return new_conn->sockfd;
}


/* binds a socket to a port always bind to all interfaces - which means addr is unused. 
returns 0 on success or negative number on failure */
int v_bind(int socket, struct in_addr addr, uint16_t port) {
   port = ntohs(port);

   pthread_mutex_lock(&c_mutex);
   connection *c = htable_get(&connections, socket);
   pthread_mutex_unlock(&c_mutex);

   if (c == NULL) {
      dbg(DBG_ERROR, "v_bind: bad socket FD: %d\n", socket);
      return -EBADF;
   }

   pthread_mutex_lock(c->protector);
   int test = c->state != CLOSED_ST; // is this connection in the TCP CLOSED state?
   int is_closed = c->is_closed; // has somebody called v_close on this socket?
   pthread_mutex_unlock(c->protector);

   if (is_closed) {
      dbg(DBG_ERROR, "v_bind: bad socket FD: %d\n", socket);
      return -EBADF;
   }

   if (test) {
      dbg(DBG_ERROR, "v_bind: socket %d not in closed state\n", socket);
      return -EINVAL;
   }

   pthread_mutex_lock(c->protector);
   c->local_port = port;
   pthread_mutex_unlock(c->protector);

   return 0;
}


/* moves socket into listen state (passive OPEN in the RFC) bind the socket to a 
random port from 1024 to 65535 that is unused if not already bound returns 0 on
success or negative number on failure */
int v_listen(int socket) {
   pthread_mutex_lock(&c_mutex);
   connection *c = htable_get(&connections, socket);
   pthread_mutex_unlock(&c_mutex);

   if (c == NULL) {
      dbg(DBG_ERROR, "v_listen: bad socket FD: %d\n", socket);
      return -EBADF;
   }

   pthread_mutex_lock(c->protector);
   int conn_state = c->state;
   int is_closed = c->is_closed;
   pthread_mutex_unlock(c->protector);

   if (is_closed) {
      dbg(DBG_ERROR, "v_listen: bad socket FD: %d\n", socket);
      return -EBADF;
   }

   state_response *r = next_state(conn_state, PSVOPEN_TR);
   if (r->new_state == -1) {
      dbg(DBG_ERROR, "v_listen: socket couldn't do passive open transition\n");
      return -EOPNOTSUPP;
   }

   pthread_mutex_lock(c->protector);
   uint16_t port = c->local_port;
   pthread_mutex_unlock(c->protector);
   if (port == 0) {
      while (port < 1024) {
         port = random_unused_port();
      }
      pthread_mutex_lock(c->protector);
      c->local_port = port;
      pthread_mutex_unlock(c->protector);
   }

   int sockfd;
   connection *conn;
   htable_iterate_begin(&connections, sockfd, conn, connection) {
      pthread_mutex_lock(conn->protector);
      int cport = conn->local_port;
      int cstate = conn->state;
      pthread_mutex_unlock(conn->protector);
      if (cport == port && cstate == LISTEN_ST) {
         dbg(DBG_ERROR, "v_listen: socket already listening on port %u\n", port);
         pthread_mutex_unlock(&c_mutex);
         return -EADDRINUSE;
      }
   } htable_iterate_end();
   pthread_mutex_unlock(&c_mutex);
   
   pthread_mutex_lock(c->protector);
   c->state = r->new_state;
   pthread_mutex_unlock(c->protector);

   free_response(r);
   return 0;
}


/* connects a socket to an address (active OPEN in the RFC) returns 0 on success or 
a negative number on failure */
// TODO: port was passed in net byte order, addr in host byte order.  correct?
int v_connect(int socket, struct in_addr addr, uint16_t port) {
   port = ntohs(port);
   addr.s_addr = ntohl(addr.s_addr);

   pthread_mutex_lock(&c_mutex);
   connection *c = htable_get(&connections, socket);
   pthread_mutex_unlock(&c_mutex);

   if (c == NULL) {
      dbg(DBG_ERROR, "v_connect: bad socket FD: %d\n", socket);
      return -EBADF;
   }
   
   pthread_mutex_lock(c->protector);
   int conn_state = c->state;
   int is_closed = c->is_closed;
   c->remote_addr = addr.s_addr;
   pthread_mutex_unlock(c->protector);

   if (is_closed) {
      dbg(DBG_ERROR, "v_connect: bad socket FD: %d\n", socket);
      return -EBADF;
   }

   int attempts = 0;
   int established = 0;
   int syn_rcvd = 0;
   struct timespec deadline;
   uint16_t p = random_unused_port();

   pthread_mutex_lock(c->protector);
   c->state = SYNSNT_ST;
   c->local_port = p;
   c->remote_port = port;
   pthread_mutex_unlock(c->protector);

   c->local_seq = random_initial_seq();

   do {
      pthread_mutex_lock(c->protector);
      int status = tcp_send_packet(c,
            //init_seq,
            c->local_seq, // seq
            0, // ack_seq
            0, // fin
            1, // syn
            0, // ack
            NULL,
            0); 
      pthread_mutex_unlock(c->protector);

      if (status == -1) {
         dbg(DBG_ERROR, "v_connect: couldn't send ctrl packet\n");
         return -1;
         //return ENETDOWN;
      }

      clock_gettime(CLOCK_REALTIME, &deadline);
      deadline.tv_sec += 3;
      // not locking here because nothing modifies conn_wait or cw_mut
      pthread_cond_timedwait(c->conn_wait, c->cw_mut, &deadline);

      pthread_mutex_lock(c->protector);
      int current_state = c->state;
      pthread_mutex_unlock(c->protector);

      if (current_state == ESTAB_ST) {
         established = 1;
         break;
      }

      if (current_state == SYNRCVD_ST) {
         syn_rcvd = 1;
         break;
      }

      attempts += 1;
   } while (attempts < 3);

   if (!established && !syn_rcvd) {
      dbg(DBG_ERROR, "v_connect: no SYNACK after 3 retries\n");
      pthread_mutex_lock(c->protector);
      c->state = CLOSED_ST;
      pthread_mutex_unlock(c->protector);
      return -ETIMEDOUT;
   }

   // TODO: could be in synrcvd state here 
   // (if the connection received SYN while we were waiting for a SYNACK)
   // how to handle?
   pthread_mutex_lock(c->protector);
   tcp_send_packet(c, 
         1 + c->local_seq, // seq
         1 + c->remote_seq, // ack_seq
         0, // fin
         0, // syn
         1, // ack
         NULL,
         0); 
   // Once established, the next segment i'll send has this seq. num:
   c->local_seq += 1;
   // And I'll expect the next one after your initial
   c->remote_seq += 1;
   
   // Translate these to the sliding window variables
   c->last_read = c->remote_seq - 1;
   //c->next_expected = c->remote_seq - 1;
   c->next_expected = c->remote_seq;
   c->last_ack = c->local_seq - 1;
   c->last_written = c->local_seq - 1;
   c->last_sent = c->local_seq - 1;

   pthread_mutex_unlock(c->protector);

   return 0;
}


void print_waiter(wt_conn *w) {
   printf("Waiter: \n");
   char sip[INET_ADDRSTRLEN];
   inet_ntop(AF_INET, &(w->s_addr), sip, INET_ADDRSTRLEN);
   printf("    s_addr = %s\n", sip);
   printf("    s_port = %u\n", w->s_port);
   char rip[INET_ADDRSTRLEN];
   inet_ntop(AF_INET, &(w->d_addr), rip, INET_ADDRSTRLEN);
   printf("    d_addr = %s\n", rip);
   printf("    d_port = %u\n", w->d_port);
   printf("     seq = %u\n", w->seq);
}


/* accept a requested connection (behave like unix socket’s accept) returns new 
socket handle on success or negative number on failure */
int v_accept(int socket, struct in_addr *node, uint16_t *port) {
   pthread_mutex_lock(&c_mutex);
   connection *c = htable_get(&connections, socket);
   pthread_mutex_unlock(&c_mutex);

   if (c == NULL) {
      dbg(DBG_ERROR, "v_accept: bad socket FD: %d\n", socket);
      return -1;
   }

   pthread_mutex_lock(c->protector);
   int conn_state = c->state;
   int is_closed = c->is_closed;
   pthread_mutex_unlock(c->protector);

   if (conn_state != LISTEN_ST) {
      dbg(DBG_ERROR, "v_accept: socket was in state %d\n", c->state);
      return -1;
   }

   if (is_closed) {
      dbg(DBG_ERROR, "v_accept: bad socket FD: %d\n", socket);
      return -1;
   }
   
   // No locking c->protector here because the queue shouldn't be changed
   wt_conn waiter;
   wt_conn *w_ptr = &waiter;
   void **p = (void **)(&w_ptr);
   bqueue_dequeue(c->wt_conns, p); // block here until connection pending

   /*
   printf("\n");
   printf("dequeued waiter: \n");
   print_waiter(w_ptr);
   */

   if (w_ptr->s_addr == 0) {
      // Listening socket was closed before anybody connected
      return -ECONNABORTED;
   }

   connection *new_conn = new_connection();
   if (new_conn == NULL) {
      dbg(DBG_ERROR, "v_accept: max connections already exist\n");
      return -1;
   }

   pthread_mutex_lock(new_conn->protector);
   new_conn->local_addr = w_ptr->d_addr;
   new_conn->remote_addr = w_ptr->s_addr;
   new_conn->local_port = w_ptr->d_port;
   new_conn->remote_port = w_ptr->s_port;
   new_conn->state = SYNRCVD_ST;
   pthread_mutex_unlock(new_conn->protector);

   pthread_mutex_lock(&c_mutex);
   htable_put(&connections, new_conn->sockfd, new_conn);
   pthread_mutex_unlock(&c_mutex);

   // Fill values to argument pointers
   if (node != NULL) {
      node->s_addr = new_conn->remote_addr;
   }
   if (port != NULL) {
      *port = new_conn->remote_port;
   }

   // Send SYN, ACK
   new_conn->local_seq = random_initial_seq();
   new_conn->remote_seq = w_ptr->seq;

   //struct in_addr addr;
   //addr.s_addr = w_ptr->s_addr;

   //char toip[INET_ADDRSTRLEN];
   //inet_ntop(AF_INET, &addr, toip, INET_ADDRSTRLEN);
   //printf("argument addr: %s\n", toip);

   pthread_mutex_lock(new_conn->protector);
   tcp_send_packet(new_conn,
         //init_seq, // seq
         new_conn->local_seq,
         //1 + w_ptr->seq, // ack_seq
         1 + new_conn->remote_seq,
         0, // fin
         1, // syn
         1, // ack
         NULL,
         0); 
   // I got your initial, so the next seq. num I expect from you is 1 + that
   new_conn->remote_seq += 1;
   // The next one I plan to send will have this seq. num:
   new_conn->local_seq += 1;
   pthread_mutex_unlock(new_conn->protector);

   // TODO: wait for ack of syn
   pthread_mutex_lock(new_conn->cw_mut);
   //   pthread_mutex_lock(new_conn->cw_mut);
   pthread_cond_wait(new_conn->conn_wait, new_conn->cw_mut);
   //   pthread_mutex_unlock(new_conn->cw_mut);
   pthread_mutex_unlock(new_conn->cw_mut);

   return new_conn->sockfd; // no lock because sockfd is never changed
}


uint32_t get_read_size(connection *c, uint32_t nbyte) {
   /*
   printf("get_read_size: last_read = %u, next_expected = %u\n",
         c->last_read, c->next_expected);
         */
   uint32_t distance = 0, lr = c->last_read, ne = c->next_expected;
   while (lr != ne) {
      distance++;
      lr++;
   }

   if (distance - 1 < nbyte) {
      //printf("returning %u\n", distance - 1);
      return distance - 1;
   }

   //printf("returning %u\n", nbyte);
   return nbyte;

   /*
   uint32_t distance;
   if (c->next_expected < c->last_read) {
      // wrap around
      //uint32_t to_end = 0xFFFFFFFF - 1 - c->last_read;
      //distance = to_end + c->next_expected;
      distance = (0xFFFFFFFF - 1 - c->last_read) + c->next_expected;
   } else {
      distance = c->next_expected - 1 - c->last_read;
   }

   if (distance < nbyte) {
      //printf(" returned %u\n", distance);
      return distance;
   }

   //printf(" returned %u\n", nbyte);
   return nbyte;
   */
}


/* read on an open socket (RECEIVE in the RFC) return num bytes read or negative 
number on failure or 0 on eof */
int v_read(int socket, unsigned char *buf, uint32_t nbyte) {
   pthread_mutex_lock(&c_mutex);
   connection *c = htable_get(&connections, socket);
   pthread_mutex_unlock(&c_mutex);

   if (c == NULL) {
      dbg(DBG_ERROR, "v_read: bad socket FD: %d\n", socket);
      return -EBADF;
   }

   pthread_mutex_lock(c->protector);

   if (c->is_closed) {
      pthread_mutex_unlock(c->protector);
      dbg(DBG_ERROR, "v_read: bad socket FD: %d\n", socket);
      return -EBADF;
   }

   if (!c->can_read) {
      pthread_mutex_unlock(c->protector);
      dbg(DBG_ERROR, "v_read: socket %d is shutdown\n", socket);
      return -1;
   }

   if (c->state == CLOSED_ST ||
         c->state == LISTEN_ST ||
         c->state == TIMEWT_ST ||
         c->state == SYNRCVD_ST ||
         c->state == SYNSNT_ST) {
      pthread_mutex_unlock(c->protector);
      dbg(DBG_ERROR, "v_read: socket %d is not in the established state\n", socket);
      return -ENOTCONN;
   }

   if (nbyte == 0) {
      pthread_mutex_unlock(c->protector);
      return 0;
   }

   if (c->state == CLOSEWT_ST || c->state == CLOSING_ST) {
      if (c->last_read == c->last_expected - 1 ||
            c->last_read == c->last_expected ) {
         /*
         printf("v_read returning 0, last_read = %u, last_expected = %u\n",
               c->last_read, c->last_expected);
         */
         pthread_mutex_unlock(c->protector);
         return 0;
      } else {
         /*
         printf("v_read not returning 0, last_read = %u, last_expected = %u\n",
               c->last_read, c->last_expected);
               */
      }
   }

   uint32_t max_can_read = get_read_size(c, nbyte);
   /*
   int max_can_read = nbyte;

   //if (c->next_expected - 1 - c->last_read < max_can_read) {
   if (less_than(c->next_expected - 1 - c->last_read, max_can_read)) {
      //max_can_read = c->next_expected - 1 - c->last_read;
      max_can_read = distance(c->next_expected - 1, c->last_read);
   }
   */

   if (max_can_read == 0) {
      // Nothing to read now, and we haven't read the last byte yet
      // So, block
      while (max_can_read == 0) {
         pthread_mutex_unlock(c->protector);
	 //	 pthread_mutex_lock(c->cw_mut);
         pthread_cond_wait(c->conn_wait, c->cw_mut);
	 //	 pthread_mutex_unlock(c->cw_mut);
         pthread_mutex_lock(c->protector);

         // Check if there's something to read
         max_can_read = get_read_size(c, nbyte);
         /*
         max_can_read = nbyte;
         if (less_than(c->next_expected - 1 - c->last_read, max_can_read)) {
            //max_can_read = c->next_expected - 1 - c->last_read;
            max_can_read = distance(c->next_expected - 1, c->last_read);
         }
         */

         if (c->last_read == c->last_expected - 1) break;
      }
   }

   if (c->state == CLOSEWT_ST || c->state == CLOSING_ST) {
      if (c->last_read == c->last_expected - 1 ||
            c->last_read == c->last_expected ) {
         /*
         printf("v_read returning 0, last_read = %u, last_expected = %u\n",
               c->last_read, c->last_expected);
         */
         pthread_mutex_unlock(c->protector);
         return 0;
      } else {
         /*
         printf("v_read not returning 0, last_read = %u, last_expected = %u\n",
               c->last_read, c->last_expected);
               */
      }
   }
 
   read_from_buffer(c->recv_buffer, c->last_read + 1, buf, max_can_read);
   c->last_read += max_can_read;

   // Send ack with updated window size
   tcp_send_packet(c, c->last_ack + 1, c->next_expected, 0, 0, 1, NULL, 0);

   //printf("end: %u, %u\n", c->last_read, c->next_expected);

   pthread_mutex_unlock(c->protector);

   return max_can_read;
}


int v_write(int socket, const unsigned char *buf, uint32_t nbyte){


  if(nbyte <= 0)
    return 0;
  
  
  pthread_mutex_lock(&c_mutex);
  connection *conn = htable_get(&connections, socket);
  pthread_mutex_unlock(&c_mutex);

  if (conn == NULL) {
    dbg(DBG_ERROR, "v_write: bad socket FD: %d\n", socket);
    return -EBADF;
   }

  pthread_mutex_lock(conn->protector);

  uint32_t max_can_write = nbyte;
  
  if(max_can_write > (BUFFER_SIZE - (conn->last_written - conn->last_ack) - 1))
    max_can_write = BUFFER_SIZE - (conn->last_written - conn->last_ack) - 1;

  while (max_can_write == 0) {
    pthread_mutex_unlock(conn->protector);
    //    pthread_mutex_lock(conn->cw_mut_write);
    pthread_cond_wait(conn->conn_wait_write, conn->cw_mut_write);
    //    pthread_mutex_unlock(conn->cw_mut_write);
    pthread_mutex_lock(conn->protector);

    // Check if there's something to read
    max_can_write = BUFFER_SIZE - (conn->last_written - conn->last_ack) - 1;
      
  }

  if(nbyte < max_can_write)
    max_can_write = nbyte;
  
  write_to_buffer(conn->send_buffer, conn->last_written+1, buf, max_can_write);

  conn->last_written += max_can_write;

  pthread_mutex_unlock(conn->protector);
  
  return max_can_write;
}


/* shutdown an open socket. If type is 1, close the writing part of the socket (CLOSE 
call in the RFC. This should send a FIN, etc.) If 2 is specified, close the reading 
part (no equivalent in the RFC; v read calls should just fail, and the window size should 
not grow any more). If 3 is specified, do both. The socket is not invalidated. returns 0 
on success, or negative number on failure. If the writing part is closed, any data not yet 
ACKed should still be retransmitted. */
int v_shutdown(int socket, int type) {
   pthread_mutex_lock(&c_mutex);
   connection *c = htable_get(&connections, socket);
   pthread_mutex_unlock(&c_mutex);

   if (c == NULL) {
      dbg(DBG_ERROR, "v_shutdown: bad socket FD: %d\n", socket);
      return -EBADF;
   }

   pthread_mutex_lock(c->protector);
   int current_state = c->state;
   int is_closed = c->is_closed;
   pthread_mutex_unlock(c->protector);

   if (is_closed) {
      dbg(DBG_ERROR, "v_shutdown: bad socket FD: %d\n", socket);
      return -EBADF;
   }

   if (current_state != ESTAB_ST && current_state != CLOSEWT_ST) {
      dbg(DBG_ERROR, "v_shutdown: socket %d not connected\n", socket);
      return -ENOTCONN;
   }

   int status;
   switch(type) {
      case 1:
         //return close_write(c);
         pthread_mutex_lock(c->protector);
         c->done_writing = 1;
         pthread_mutex_unlock(c->protector);
         if (wrote_all_bytes(c)) {
            close_write(c);
         }
         return 0;
      case 0:
         return close_read(c);
      case 2:
         status = close_read(c);
         if (status < 0) return status;
         //return close_write(c);
         pthread_mutex_lock(c->protector);
         c->done_writing = 1;
         pthread_mutex_unlock(c->protector);
         if (wrote_all_bytes(c)) {
            close_write(c);
         }
         return 0;
      default:
         dbg(DBG_ERROR, "v_shutdown: unknown type %d\n", type);
         return -1;
   }
}


int wrote_all_bytes(connection *c) {
   int rtn = (c->done_writing && c->last_ack == c->last_written);

   return rtn;
}


int close_write(connection *c) {
   pthread_mutex_lock(c->protector);
   int current_state = c->state;
   pthread_mutex_unlock(c->protector);

   state_response *r = next_state(current_state, CLOSE_TR);
   if (r->new_state == -1) {
      dbg(DBG_ERROR, "v_shutdown: socket couldn't handle CLOSE_TR transition\n");
      return -1;
   }

   pthread_mutex_lock(c->protector);
   //printf("sending FIN: SEQ = %u, ACK_SEQ = %u\n", c->last_ack, c->next_expected);
   int do_fin = 0;
   if (c->state == ESTAB_ST || c->state == CLOSEWT_ST) {
      do_fin = 1;
   }

   printf("close_write sending ACK: SEQ = %u, ACK_SEQ = %u\n", c->last_ack + 1, c->next_expected);
   tcp_send_packet(c,
      c->last_ack + 1,
      c->next_expected,
      do_fin, // fin
      0, // syn
      1, // ack
      NULL,
      0); 

   if (do_fin) {
      // FIN is 1 byte
      c->last_written += 1;
      c->last_sent += 1;
   }

   c->state = r->new_state;
   pthread_mutex_unlock(c->protector);
   return 0;
}


void close_read(connection *c) {
   pthread_mutex_lock(c->protector);
   c->can_read = 0;
   pthread_mutex_unlock(c->protector);
   return 0;
}


/* Invalidate this socket, making the underlying connection inaccessible to any of 
these API functions. If the writing part of the socket has not been shutdown yet, 
then do so. The connection shouldn’t be terminated, though; any data not yet ACKed
should still be retransmitted. */
int v_close(int socket) {
   pthread_mutex_lock(&c_mutex);
   connection *c = htable_get(&connections, socket);
   pthread_mutex_unlock(&c_mutex);

   if (c == NULL) {
      dbg(DBG_ERROR, "v_close: bad socket FD: %d\n", socket);
      return -EBADF;
   }

   pthread_mutex_lock(c->protector);
   int current_state = c->state;
   int is_closed = c->is_closed;
   pthread_mutex_unlock(c->protector);

   if (is_closed) {
      dbg(DBG_ERROR, "v_close: bad socket FD: %d\n", socket);
      return -EBADF;
   }
   pthread_mutex_lock(c->protector);
   c->done_writing = 1;
   c->is_closed = 1;
   pthread_mutex_unlock(c->protector);

/*    if (current_state == ESTAB_ST || current_state == CLOSEWT_ST) { */
/*       close_write(c); */
/*    } */

/*    pthread_mutex_lock(c->protector); */
/*    c->is_closed = 1; */
/*    pthread_mutex_unlock(c->protector); */

/*    pthread_mutex_lock(c->protector); */
/*    current_state = c->state; */
/*    pthread_mutex_unlock(c->protector); */
   if (wrote_all_bytes(c)) {
      close_write(c);
   }

/*    if (current_state == CLOSED_ST ||  */
/*          current_state == LISTEN_ST || */
/*          current_state == SYNRCVD_ST || */
/*          current_state == SYNSNT_ST) { */
/*       pthread_mutex_lock(&c_mutex); */
/*       htable_remove(&connections, socket); */
/*       pthread_mutex_unlock(&c_mutex); */
/*       free_connection(c); */
/*    } */

   return 0;
}


/* free heap memory associated with c */
void free_connection(connection *c) {

   // I am very, very sorry about this
   struct timespec deadline;
   while (c->thread_running) {
      clock_gettime(CLOCK_REALTIME, &deadline);
      deadline.tv_nsec += 1000;
      nanosleep(&deadline, NULL);
   }
   printf("free connection done waiting\n");


   pthread_mutex_destroy(c->protector);
   free(c->protector);

   pthread_cond_destroy(c->conn_wait);
   free(c->conn_wait);

   pthread_mutex_destroy(c->cw_mut);
   free(c->cw_mut);

   bqueue_destroy(c->wt_conns);
   free(c->wt_conns);

   pthread_cond_destroy(c->conn_wait_write);
   free(c->conn_wait_write);

   pthread_mutex_destroy(c->cw_mut_write);
   free(c->cw_mut_write);

   pthread_cond_destroy(c->conn_wait_write2);
   free(c->conn_wait_write2);

   pthread_mutex_destroy(c->cw_mut_write2);
   free(c->cw_mut_write2);

   free_buffer_list(c->recv_root);
   
   free_retran_node(c->send_root);

   free(c->recv_buffer);

   free(c);
}


void free_buffer_list(buffer_list *root) {
  buffer_list *prev = root;
  buffer_list *curr = root->next;
  while(curr != NULL){
    free(prev);
    prev = curr;
    curr = curr->next;
  }
}

int free_retran_node(retran_node *root){

  retran_node *curr = dequeue(root);
  while(curr != NULL ){
    free(curr);
    curr = dequeue(root);
  }

  free(root);
  return 1;
}
