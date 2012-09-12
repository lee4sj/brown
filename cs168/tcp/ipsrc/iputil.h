#ifndef IPUTIL_H
#define IPUTIL_H

#include <netinet/in.h>
#include "netlayer.h" //for struct ip *

struct ip *ip_packet_construct(
    struct in_addr src, struct in_addr dest,
    int DNF, uint8_t timeToLive, uint8_t protocol, 
    void *data, uint16_t datalen);
void ip_packet_destruct(struct ip *);
void ip_packet_hton(struct ip *);
void ip_packet_ntoh(struct ip *);
int ip_calc_checksum(struct ip *packet);
void ip_packet_print(struct ip *packet, int printData);

char *inet_ntoa_host(struct in_addr a);

#endif
