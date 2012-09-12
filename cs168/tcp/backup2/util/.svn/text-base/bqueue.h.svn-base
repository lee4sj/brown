#ifndef __BQUEUE_H__
#define __BQUEUE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <pthread.h>
#include <assert.h>
#include <time.h>
#include "list.h"

typedef struct bqueue {
    pthread_mutex_t q_mtx;              /* for synchronization */
    pthread_cond_t q_cond;              /* for a-wakin' up */
    list_t q_list;                      /* stores the data */
    int q_status;                       /* 0 if init'd, 1 if destroyed */
    int q_count;                        /* count of waiting threads */
} bqueue_t;

typedef struct bqueue_item {
    void *qi_data;                      /* holds the data */
    list_link_t qi_link;                /* to be able to place it in a list */
} bqueue_item_t;

int bqueue_init( bqueue_t *q );
int bqueue_destroy( bqueue_t *q );
int bqueue_enqueue( bqueue_t *q, void *data );
int bqueue_dequeue( bqueue_t *q, void **data );

//given a relative timeout, fill in the absolute timeout
void bqueue_calc_abs_timespec(const struct timespec *rel_timeout,
                              struct timespec *abs_timeout);

/* returns -ETIMEDOUT if data couldn't be gotten within the relative timeout
   returns -EINVAL if the queue is destroyed while reading
   returns 0 otherwise 

   bqueue_timed_dequeue_rel is given a *relative* time. That means
   that if rel_timeout represents 3 seconds, it will attempt to
   dequeue for 3 seconds.  bqueue_timed_dequeue_abs takes an
   *absolute* timeout. If abs_ts is 3 seconds, it will mean 3 seconds
   after the start of the epoch. If you want to try dequeuing for 3
   seconds, you must first get the current time and add 3 seconds onto
   it. You can use bqueue_calc_abs_timespec to do this, or you can
   just use bqueue_timed_dequeue_ rel.

   _abs is useful because say you're waiting for 20 seconds for a
   valid ACK. You might get other packets that are invalid in this
   time. If you just use _rel, then every time dequeue returns you'd
   have to calculate how much time you waited and subtract it from the
   timeout. _abs saves you from having to do this yourself, and is
   more accurate as well.
   */
int bqueue_timed_dequeue_abs( bqueue_t *q, void ** data,
                              const struct timespec *abs_ts);
int bqueue_timed_dequeue_rel( bqueue_t *q, void **data, 
                              const struct timespec *rel_timeout);
int bqueue_empty( bqueue_t *q );
int bqueue_trydequeue( bqueue_t *q, void **data );

#ifdef __cplusplus
}
#endif

#endif /* __BQUEUE_H__ */
