//TCP Thread
#ifndef TCP_THREAD_H
#define TCP_THREAD_H

#include <pthread.h>
#include "socket_api.h"


retran_node* dequeue(retran_node *root);

retran_node* enqueue(retran_node *root, retran_node *node);

int calc_SRRT(struct timespec *srrt, struct timespce rrt);

void* run_tcp_thread(connection* conn);



#endif
