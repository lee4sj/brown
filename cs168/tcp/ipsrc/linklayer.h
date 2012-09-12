#ifndef LINKLAYER_H
#define LINKLAYER_H

#include "interface.h"

#define LINK_MTU 1400

/* BEGIN SOLUTION */
int link_up(interface_t *);
int link_down(interface_t *);
/* END SOLUTION */
ssize_t link_send(interface_t* netif, const char* buf, size_t len);
ssize_t link_recv(interface_t* netif, char* buf, size_t len);

#endif
