#include "bqueue.h"
#include "list.h"
#include "dbg.h"
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>

static int __bqueue_dequeue( bqueue_t *q, void **data,
                             const struct timespec *rel_timeout );

/* initialize a blocking queue */
int bqueue_init( bqueue_t *q ) {
    pthread_mutex_init( &q->q_mtx, NULL );
    
    pthread_cond_init( &q->q_cond, NULL );

    list_init( &q->q_list );
    q->q_status = q->q_count = 0;

    return 0;
}

/* destroy a blocking queue */
int bqueue_destroy( bqueue_t *q ) {
    bqueue_item_t *qi;

    pthread_mutex_lock( &q->q_mtx );

    assert( !q->q_status );

    q->q_status = 1;
    pthread_cond_broadcast( &q->q_cond );

    /* wait for all bqueue_debqueue-ing threads to wake and notice that
       the bqueue is going away */
    while( q->q_count )
        pthread_cond_wait( &q->q_cond, &q->q_mtx );

    /* free all list elements, leaving data */
    list_iterate_begin( &q->q_list, qi, bqueue_item_t, qi_link ) {
        free( qi );
    } list_iterate_end();

    return 0;
}

/* enqueue an item into the queue. if the queue has been
 * destroyed, returns -EINVAL */
int bqueue_enqueue( bqueue_t *q, void *data ) {
    int ret;
    bqueue_item_t *qi;
   
    assert( data );
  
    pthread_mutex_lock( &q->q_mtx );

    if( q->q_status ) {
        pthread_mutex_unlock( &q->q_mtx );

        ret = -EINVAL;
    }
    else {
        qi = (bqueue_item_t*) malloc( sizeof( bqueue_item_t ) );
        qi->qi_data = data;
        list_link_init( &qi->qi_link );

        list_insert_tail( &q->q_list, &qi->qi_link );

        pthread_cond_signal( &q->q_cond );
        pthread_mutex_unlock( &q->q_mtx );

        ret = 0;
    }

    return ret;
}

/* dequeues an item from the queue... if the queue has been destroyed or
 * is destroyed while we're blocked, returns -EINVAL */
int bqueue_dequeue( bqueue_t *q, void **data ) {
    return bqueue_timed_dequeue_abs( q, data, NULL );
}

void bqueue_calc_abs_timespec(const struct timespec *rel_timeout,
                              struct timespec *abs_ts) {
  struct timeval cur_tv;
  gettimeofday(&cur_tv, NULL);
  abs_ts->tv_sec = cur_tv.tv_sec + rel_timeout->tv_sec;
  abs_ts->tv_nsec = cur_tv.tv_usec*1000 + rel_timeout->tv_nsec;
  if (abs_ts->tv_nsec > 1000000000) { 
    abs_ts->tv_nsec -= 1000000000;
    abs_ts->tv_sec += 1;
  }
}

/* returns -ETIMEDOUT if data couldn't be gotten within the relative timeout
   returns -EINVAL if the queue is destroyed while reading
   returns 0 otherwise 
   */
int bqueue_timed_dequeue_abs( bqueue_t *q, void ** data,
                              const struct timespec *abs_ts) {
  int ret;
  if (abs_ts != NULL) {
    ret = pthread_mutex_timedlock( &q->q_mtx, abs_ts );
    if (ret == ETIMEDOUT) 
      return -ETIMEDOUT;

    ret = __bqueue_dequeue( q, data , abs_ts );
    pthread_mutex_unlock( &q->q_mtx );
    return ret;
  }

  pthread_mutex_lock( &q->q_mtx );
  ret = __bqueue_dequeue( q, data , NULL );
  pthread_mutex_unlock( &q->q_mtx );
  return ret;
}

int bqueue_timed_dequeue_rel( bqueue_t *q, void **data, 
                              const struct timespec *rel_timeout) {
  if (rel_timeout != NULL) {
    struct timespec abs_ts;
    bqueue_calc_abs_timespec(rel_timeout, &abs_ts);
    return bqueue_timed_dequeue_abs(q, data, &abs_ts);
  }
  return bqueue_timed_dequeue_abs(q, data, NULL);
}
  

/* returns 1 if the queue is empty, 0 otherwise */
int bqueue_empty( bqueue_t *q ) {
    int ret;

    pthread_mutex_lock( &q->q_mtx );
    ret = list_empty( &q->q_list );
    pthread_mutex_unlock( &q->q_mtx );

    return ret;
}

/* attempts to dequeue an item from the queue... if there are no items
 * in the queue, rather than blocking we simply return 1 and *data has
 * an undefined value */
int bqueue_trydequeue( bqueue_t *q, void **data ) {
    int ret;

    pthread_mutex_lock( &q->q_mtx );

    if( !list_empty( &q->q_list ) )
        ret = __bqueue_dequeue( q, data, NULL );
    else
        ret = 1;

    pthread_mutex_unlock( &q->q_mtx );

    return ret;
}

/* dequeues an item from the queue... if the queue has been destroyed or
 * is destroyed while we're blocked, returns -EINVAL */
int __bqueue_dequeue( bqueue_t *q, void **data, 
                      const struct timespec *abs_timeout ) {
    int ret;
    bqueue_item_t *qi;

    q->q_count++;

    while( list_empty( &q->q_list ) && !q->q_status )
    {
      if (abs_timeout != NULL) 
      {
        dbg(DBG_UTIL, "__bqueue_dequeue: timeout = %d, %d\n", 
            abs_timeout->tv_sec, abs_timeout->tv_nsec);
        ret = pthread_cond_timedwait( &q->q_cond, &q->q_mtx, abs_timeout ); 
        if (ret == ETIMEDOUT)
        {
          dbg(DBG_UTIL, "__bqueue_dequeue: cond timed out\n");
          return -ETIMEDOUT;
        }
      }
      else
      {
        dbg(DBG_UTIL, "__bqueue_dequeue: no timeout\n");
        pthread_cond_wait( &q->q_cond, &q->q_mtx );
      }
    }

    q->q_count--;

    /* the queue is being destroyed */
    if( q->q_status ) {
        if( q->q_count == 0 )
            pthread_cond_signal( &q->q_cond );

        ret = -EINVAL;
    }
    else {
        qi = list_head( &q->q_list, bqueue_item_t, qi_link );
        list_remove_head( &q->q_list );
        *data = qi->qi_data;
        free( qi );

        ret = 0;
    }

    return ret;
}
