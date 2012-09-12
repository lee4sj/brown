#ifndef __DNS_BUILDER_H
#define __DNS_BUILDER_H

#include <stdint.h>

struct dns_header{
  uint16_t id;
  uint16_t tags;
  uint16_t num_q;
  uint16_t num_anws;
  uint16_t num_ns;
  uint16_t num_addi;
};

struct resrc_rec{
  uint16_t type;
  uint16_t class;
  uint32_t ttl;
  uint16_t rlength;
};



/*
 */
char *build_dns_packet(uint16_t id, uint16_t flags, uint16_t num_q,
		       uint16_t num_anws, uint16_t num_ns, uint16_t num_addi,
		       char *name,
		       const char *payload, int payload_size);
#endif
