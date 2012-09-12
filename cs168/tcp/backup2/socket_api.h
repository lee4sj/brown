#ifndef SOCKET_API_H
#define SOCKET_API_H

#include <pthread.h>
#include <stdint.h>
#include <arpa/inet.h>
#include "util/bqueue.h"
#include "util/htable.h"

// This could be changed later
#define MAX_CONNECTIONS 16
#define BUFFER_SIZE /*0x8000*/0x10000
#define MAX_SEG_SIZE 1024
#define MAX_TIMEOUT 60
#define MIN_TIMEOUT_SEC 0
#define MIN_TIMEOUT_NSEC 10000000
#define SRTT_FACTOR 0.8
#define TIMEOUT_FACTOR 2
#define KEEP_ALIVE_TIME 10

typedef struct buffer_list {
  uint32_t seq;
  uint16_t size;
  struct buffer_list* next;
} buffer_list;

typedef struct retran_node{
  uint32_t seq;
  uint16_t size;
  int add_to_srtt;
  struct timespec timestamp;
  struct timespec sent_time;
  struct retran_node *prev; //end of the node for the root node.
  struct retran_node *next;
} retran_node;

typedef struct connection {
  pthread_mutex_t *protector; // must be held when changing any connection state

  int sockfd;
  int state;
  int can_read; // for shutdown type 2 command (changes this flag to 0)
  int is_closed; // closed by v_close, not the TCP state
  int n_timeouts; // to handle transitioning out of TIMEWT state
  int done_writing;
  int thread_running;
  //int saw_eof; // initially 0.  set to 1 when receive FIN. used to unblock threads in v_read

  uint32_t local_addr;  // probably unneeded? you bind to all interfaces
  uint16_t local_port;
  uint32_t remote_addr;
  uint16_t remote_port;

  uint32_t remote_init_seq; // this value is set when we get a SYNACK

  uint32_t local_seq;
  uint32_t remote_seq; // this value is set when we get a SYNACK
                        // next expected byte from the other side


  //needed for data transfer:
  //sending window
  uint32_t last_ack;      //last acked byte.
  uint32_t last_written;     //last byte written to the buffer.
  uint32_t last_sent;     //last byte sent.
  retran_node *send_root; //this node keeps prev as the end node

  struct timespec SRTT;
  struct timespec keep_alive;

  uint16_t window_size;

  //receiving window
  uint32_t last_read;     //last byte read.
  uint32_t next_expected;     //next byte expected.
  uint32_t last_expected; // seq number of final byte of data expected (known after seeing FIN)
  //  uint32_t last_received;     //last byte received.
  buffer_list *recv_root;  //to keep track of recved data

  char *send_buffer;
  char *recv_buffer;

  // Used to block in v_connect() until the SYNACK is received
  //pthread_cond_t *got_synack;
  pthread_cond_t *conn_wait;
  //pthread_mutex_t *sa_mut;
  pthread_mutex_t *cw_mut;

  pthread_cond_t *conn_wait_write;
  pthread_mutex_t *cw_mut_write;

  
  pthread_cond_t *conn_wait_write2;
  pthread_mutex_t *cw_mut_write2;

  /*
   * Queue of waiting connections
   */
  bqueue_t *wt_conns; // queue of waiting connections
  // receipt of SYN packets to our port adds stuff to this

} connection;


/* represents a pending connection - appears on the blocking queue
 * call to accept() will create a new socket from this */
typedef struct wt_conn {
   uint32_t s_addr;
   uint16_t s_port;
   uint32_t d_addr;
   uint16_t d_port;
   uint32_t seq; // initial seq
} wt_conn;


/* initialize connection table */
void socket_layer_init();


/* returns pointer to sockets hash table */
htable_t *get_sockets();


/* returns pointer to connections table mutex */
pthread_mutex_t *get_connections_mutex();


/* returns a new, unbound socket. on failure, returns a negative value */
int v_socket();


/* binds a socket to a port always bind to all interfaces - which means addr is unused. 
returns 0 on success or negative number on failure */
int v_bind(int socket, struct in_addr addr, uint16_t port);


/* moves socket into listen state (passive OPEN in the RFC) bind the socket to a 
random port from 1024 to 65535 that is unused if not already bound returns 0 on
success or negative number on failure */
int v_listen(int socket);


/* connects a socket to an address (active OPEN in the RFC) returns 0 on success or 
a negative number on failure */
int v_connect(int socket, struct in_addr addr, uint16_t port);


/* accept a requested connection (behave like unix socket’s accept) returns new 
socket handle on success or negative number on failure */
int v_accept(int socket, struct in_addr *node, uint16_t *port);


/* read on an open socket (RECEIVE in the RFC) return num bytes read or negative 
number on failure or 0 on eof */
int v_read(int socket, unsigned char *buf, uint32_t nbyte);


/* write on an open socket (SEND in the RFC) return num bytes written or negative 
number on failure */
int v_write(int socket, const unsigned char *buf, uint32_t nbyte);


/* shutdown an open socket. If type is 1, close the writing part of the socket (CLOSE 
call in the RFC. This should send a FIN, etc.) If 2 is specified, close the reading 
part (no equivalent in the RFC; v read calls should just fail, and the window size should 
not grow any more). If 3 is specified, do both. The socket is not invalidated. returns 0 
on success, or negative number on failure If the writing part is closed, any data not yet 
ACKed should still be retransmitted. */
int v_shutdown(int socket, int type);


/* Invalidate this socket, making the underlying connection inaccessible to any of 
these API functions. If the writing part of the socket has not been shutdown yet, 
then do so. The connection shouldn’t be terminated, though; any data not yet ACKed
should still be retransmitted. */
int v_close(int socket);


/* self explanatory */
uint16_t random_unused_port();


/* free heap memory associated with c */
void free_connection(connection *c); 

#endif
