#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#include "tcp_thread.h"
#include "socket_api.h"
#include "transport_layer.h"
#include "state_machine.h"
#include "util/dbg.h"


#define round(x) ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))

retran_node *remove_node(retran_node *root, retran_node *node){

  if(root == node)
    return NULL;

  retran_node *prev = node->prev;
  retran_node *next = node->next;
  
  prev->next = node->next;
  next->prev = node->prev;

  return node;
}

retran_node *dequeue(retran_node *root){

  retran_node *curr = root->next;

  if(curr == root)
    return NULL;

  root->next = curr->next;
  root->next->prev = root;
  
  return curr;
}

retran_node *peek(retran_node *root){
  retran_node *curr = root->next;

  if(curr == root)
    return NULL;

  return curr;
}

retran_node *enqueue(retran_node *root, retran_node *node){

  node->next = root;
  node->prev = root->prev;

  root->prev->next = node;
  root->prev = node;
  
  return node;
}

//timespec less_than
int time_lt(struct timespec *a, struct timespec *b){

  /*  if(b->tv_sec == 0)
      return 0;*/
  
  if(a->tv_sec > b->tv_sec)
    return 0;

  if(a->tv_sec < b->tv_sec)
    return 1;

  if(a->tv_nsec < b->tv_nsec)
    return 1;

  return 0;
}

struct timespec *sub_time(struct timespec *a, struct timespec *b){
  a->tv_sec = a->tv_sec - b->tv_sec;
  a->tv_nsec = a->tv_nsec - b->tv_nsec;

  if(a->tv_nsec < 0){
    a->tv_sec -= 1;
    a->tv_nsec += 1000000000;
  }

  else if(a->tv_nsec >= 1000000000){
    a->tv_sec += 1;
    a->tv_nsec -= 1000000000;
  }

  return a;
}

struct timespec *add_time(struct timespec *a, struct timespec *b){
  a->tv_sec += b->tv_sec;
  a->tv_nsec += b->tv_nsec;

  
  if(a->tv_nsec < 0){
    a->tv_sec -= 1;
    a->tv_nsec += 1000000000;
  }

  else if(a->tv_nsec >= 1000000000){
    a->tv_sec += 1;
    a->tv_nsec -= 1000000000;
  }

  return a;
  
}

int calc_SRTT(struct timespec *srtt, struct timespec *rtt){
  srtt->tv_sec = round((double)srtt->tv_sec * SRTT_FACTOR + (1-SRTT_FACTOR) * (double)rtt->tv_sec);
  srtt->tv_nsec = round((double)srtt->tv_sec * SRTT_FACTOR + (1 - SRTT_FACTOR) * (double)rtt->tv_nsec);

  if(srtt->tv_nsec < 0){
    srtt->tv_sec -= 1;
    srtt->tv_nsec += 1000000000;
  }

  else if(srtt->tv_nsec >= 1000000000){
    srtt->tv_sec += 1;
    srtt->tv_nsec -= 1000000000;
  }

  return 1;
}

struct timespec *get_retran_time(struct timespec *dest, struct timespec *srtt){

  dest->tv_sec = TIMEOUT_FACTOR * srtt->tv_sec;
  dest->tv_nsec = TIMEOUT_FACTOR * srtt->tv_nsec;
  if(dest->tv_sec <= MIN_TIMEOUT_SEC && dest->tv_nsec <= MIN_TIMEOUT_NSEC){
    dest->tv_sec = MIN_TIMEOUT_SEC;
    dest->tv_nsec = MIN_TIMEOUT_NSEC;
    return dest;
  }

  if(dest->tv_sec > MAX_TIMEOUT || (dest->tv_sec == MAX_TIMEOUT - 1 && dest->tv_nsec >= 1000000000)){
    dest->tv_sec = MAX_TIMEOUT;
    dest->tv_nsec = 0;
    return dest;
  }

  return dest;
}

void* run_tcp_thread(connection* conn){

  struct timespec keep_alive_timeout;
  struct timespec keep_alive;
  struct timespec curr_time;
  struct timespec rto;
  
  uint16_t window_size;
  uint32_t last_ack;
  uint32_t last_sent;
  uint32_t last_written;
  uint16_t data_size;
  int sent_data;
  
  for(;;){

    sent_data = 0;


    

    
    pthread_mutex_lock(conn->protector);


    if (conn->state == FINWT2_ST || conn->state == CLOSED_ST || conn->state == TIMEWT_ST) {
       if (wrote_all_bytes(conn)) {
         printf("tcp thread exiting\n");
         conn->thread_running = 0;
	 pthread_mutex_unlock(conn->protector);
         pthread_exit(NULL);
	      return;
       }
    }

    window_size = conn->window_size;
    last_ack = conn->last_ack;
    last_sent = conn->last_sent;
    last_written = conn->last_written;


    
    retran_node *curr = peek(conn->send_root);
    //    retran_node *curr = conn->send_root->next;

    clock_gettime(CLOCK_REALTIME, &curr_time);
    
    //    printf("%u, %u\n", get_retran_time(conn->SRTT), curr_time.tv_sec);

    while(curr != NULL && time_lt(&curr->timestamp, &curr_time)){

      //      dbg(DBG_ERROR, "timestamp: %d, currtime: %d\n", curr->timestamp.tv_sec, curr_time.tv_sec);
      
      dequeue(conn->send_root);
      sent_data = 1;

      if((curr->seq+curr->size) % BUFFER_SIZE < curr->seq % BUFFER_SIZE){

	char buf[curr->size];
	uint32_t temp_size = BUFFER_SIZE - (curr->seq % BUFFER_SIZE);

	memcpy(buf, conn->send_buffer + (curr->seq % BUFFER_SIZE), temp_size );
	memcpy(buf + temp_size, conn->send_buffer, curr->size - temp_size);

	
	tcp_send_packet(conn, curr->seq, conn->next_expected,
			0,0,1, buf, curr->size);
      }
      else
	tcp_send_packet(conn, curr->seq, conn->next_expected,
			0, 0, 1, &(conn->send_buffer[curr->seq%BUFFER_SIZE]), curr->size);
      
      
      curr->add_to_srtt = 0;
      clock_gettime(CLOCK_REALTIME, &curr->timestamp);
      get_retran_time(&rto, &conn->SRTT);
      add_time( &curr->timestamp, &rto);

      dbg(DBG_ERROR, "resent %d: %d: %u: %u\n", rto.tv_sec, curr_time.tv_sec, curr->seq, curr->size); 
      
      //queue it to the next one
      enqueue(conn->send_root, curr);
      curr = peek(conn->send_root);

      //curr = curr->next;

      clock_gettime(CLOCK_REALTIME, &curr_time);

    }


    

    pthread_mutex_unlock(conn->protector);
    //    while(0 < window_size - (last_sent - last_ack) && window_size - (last_sent - last_ack) < 32768 &&  last_written > last_sent){
    //    printf("win:%u, last_sent:%u, last_ack:%u, last_written:%u\n", window_size, last_sent, last_ack, last_written);
    while(window_size > (last_sent - last_ack) &&  less_than(last_sent, last_written)){

      sent_data = 1;
      
      data_size = last_written - last_sent;
      if(data_size > window_size)
	data_size = window_size;
      if(data_size > MAX_SEG_SIZE)
	data_size = MAX_SEG_SIZE;


      pthread_mutex_lock(conn->protector);

      if((last_sent+1+data_size) % BUFFER_SIZE < (last_sent+1) % BUFFER_SIZE){

	char buf[data_size];
	uint32_t temp_size = BUFFER_SIZE - ((last_sent + 1) % BUFFER_SIZE);

	memcpy(buf, &(conn->send_buffer[ (last_sent + 1) % BUFFER_SIZE]), temp_size );
	memcpy(buf + temp_size, conn->send_buffer, data_size - temp_size);

	tcp_send_packet(conn, last_sent + 1, conn->next_expected,
			0,0,1, buf, data_size );

      }
      else
	tcp_send_packet(conn, last_sent+1, conn->next_expected,
			0, 0, 1, &(conn->send_buffer[(last_sent+1)%BUFFER_SIZE]), data_size);


      dbg(DBG_ERROR, "sent last_sent: %u, last_ack: %u \n", last_sent, last_ack);
     

      if (conn->state == FINWT2_ST || conn->state == CLOSED_ST || conn->state == TIMEWT_ST) {
	if (wrote_all_bytes(conn)) {
	  printf("tcp thread exiting\n");
	  conn->thread_running = 0;
	  pthread_mutex_unlock(conn->protector);
	  pthread_exit(NULL);
	  return;
	}
      }
    
    
      //add to send_root, the retransmission
      retran_node *curr = (retran_node *) malloc(sizeof(retran_node));
      curr->seq = last_sent + 1;
      curr->size = data_size;
      curr->add_to_srtt = 1;
      clock_gettime(CLOCK_REALTIME, &curr->timestamp);
      clock_gettime(CLOCK_REALTIME, &curr->sent_time);
      get_retran_time(&rto, &conn->SRTT);
      add_time(&curr->timestamp, &rto);

      enqueue(conn->send_root, curr);
      
      last_sent += data_size;


      window_size = conn->window_size;
      last_ack = conn->last_ack;
      conn->last_sent = last_sent;

/*       if(DEBUG_MODE_TCP) */
      //printf("sent packet: %u: %u: %u: %u\n", last_written, last_sent, last_ack, window_size); 

      pthread_mutex_unlock(conn->protector);
    }
    pthread_mutex_lock(conn->protector);

   

    if (conn->state == FINWT2_ST || conn->state == CLOSED_ST || conn->state == TIMEWT_ST) {
       if (wrote_all_bytes(conn)) {
         printf("tcp thread exiting\n");
         conn->thread_running = 0;
	 pthread_mutex_unlock(conn->protector);
         pthread_exit(NULL);
	      return;
       }
    }
    


    //KEEP ALIVE
    if(!sent_data){
      keep_alive.tv_sec = conn->keep_alive.tv_sec + KEEP_ALIVE_TIME;
      keep_alive.tv_nsec = conn->keep_alive.tv_nsec;
      clock_gettime(CLOCK_REALTIME, &curr_time);
      if(time_lt(&keep_alive, &curr_time)){
	printf("keep alive\n");
	tcp_send_packet(conn, conn->last_sent+1, conn->next_expected , 0, 0, 1, NULL, 0);
	clock_gettime(CLOCK_REALTIME, &conn->keep_alive);
      }
    }


    
    
    if(conn->send_root->next != conn->send_root){
      keep_alive_timeout.tv_sec = conn->send_root->next->timestamp.tv_sec;
      keep_alive_timeout.tv_nsec = conn->send_root->next->timestamp.tv_nsec;
    }
    else{
      clock_gettime(CLOCK_REALTIME, &keep_alive_timeout);
      struct timespec temp1;
      temp1.tv_sec = 0;
      temp1.tv_nsec = 10000000;
      add_time(&keep_alive_timeout, &temp1);
    }



    pthread_mutex_unlock(conn->protector);
    

    //    pthread_mutex_lock(conn->cw_mut_write2);
    int suc = pthread_cond_timedwait(conn->conn_wait_write2, conn->cw_mut_write2, &keep_alive_timeout);
    /*    if(suc != 0)
	  dbg(DBG_ERROR, "unsuccessful timedwait!\n");*/
    //    pthread_mutex_unlock(conn->cw_mut_write2);



/*     //it seems that broadcasting is not waking this up! */
/*     clock_gettime(CLOCK_REALTIME, &keep_alive_timeout); */
/*     keep_alive_timeout.tv_sec = keep_alive_timeout.tv_sec + KEEP_ALIVE_TIME; */
/*     pthread_cond_timedwait(conn->conn_wait_write, conn->cw_mut_write, &keep_alive_timeout); */

/*     if(DEBUG_MODE_TCP) */
/*       printf("woke up!!\n"); */

/*       //   KEEP_ALIVE */
/*     clock_gettime(CLOCK_REALTIME, &curr_time); */
/*     if(time_lt(keep_alive_timeout, curr_time)){ */
/*       pthread_mutex_lock(conn->protector); */
/*       tcp_send_packet(conn, conn->last_sent+1, conn->next_expected , 0, 0, 1, NULL, 0); */
/*       pthread_mutex_unlock(conn->protector); */
/* /\*       if(DEBUG_MODE_TCP) *\/ */
/* /\* 	printf("keep alive: %u, curr_time: %u\n", keep_alive_timeout.tv_sec, curr_time.tv_sec); *\/ */
/*     } */

    
  }
}
