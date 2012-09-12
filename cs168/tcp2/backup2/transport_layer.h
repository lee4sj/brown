#ifndef TRANSPORT_LAYER_H
#define TRANSPORT_LAYER_H
#include <arpa/inet.h>
#include <pthread.h>
#include "socket_api.h"
#include "util/htable.h"

#define TCP_PROTOCOLNUM 6

/*
typedef struct tcphdr
  {
    u_int16_t source;
    u_int16_t dest;
    u_int32_t seq;
    u_int32_t ack_seq;
#  if __BYTE_ORDER == __LITTLE_ENDIAN
    u_int16_t res1:4;
    u_int16_t doff:4;
    u_int16_t fin:1;
    u_int16_t syn:1;
    u_int16_t rst:1;
    u_int16_t psh:1;
    u_int16_t ack:1;
    u_int16_t urg:1;
    u_int16_t res2:2;
#  elif __BYTE_ORDER == __BIG_ENDIAN
    u_int16_t doff:4;
    u_int16_t res1:4;
    u_int16_t res2:2;
    u_int16_t urg:1;
    u_int16_t ack:1;
    u_int16_t psh:1;
    u_int16_t rst:1;
    u_int16_t syn:1;
    u_int16_t fin:1;
#  else
#   error "Adjust your <bits/endian.h> defines"
#  endif
    u_int16_t window;
    u_int16_t check;
    u_int16_t urg_ptr;
} tcphdr;
*/


/*
 * Called by v_connect.  Corresponds to handling the SENDSYN_AC action, the first
 * part of the 3-way handshake.
 * Create a TCP packet with a random sequence number, the SYN flag set, and send it
 * to addr.
 * TODO: 3 second timeout?
 */
/*
int send_syn(connection *c, struct in_addr addr, uint16_t port);

int send_ack(connection *c, struct in_addr addr, uint16_t port);
*/

/*
 * Send a TCP control packet, no data, with the specified flags.
 */
int tcp_send_packet(connection *c, 
      uint32_t seq,
      uint32_t ack_seq,
      int fin,
      int syn,
      int ack,
      char *data,
      unsigned int data_len);


int read_from_buffer(char *buffer, uint32_t offset, char *from, uint16_t size); 


void transport_layer_init(char *lnxfile);

list_t *get_interface_list();

pthread_mutex_t *get_interface_mutex();


#endif
