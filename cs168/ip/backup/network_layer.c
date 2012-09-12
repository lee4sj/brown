#include <stdio.h>
#include <stdlib.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <string.h>
#include "network_layer.h"
#include "link_layer.h"
#include "util/ipsum.h"
#include "util/htable.h"
#include "rip.h"


/*
 * Global variables to hold handler hash table and mutex
 */
htable_t handlers;
pthread_mutex_t handlers_mutex;

/*
 * Returns 1 if packet destined for up local interface
 * -1 if destined for down local interface
 *  0 if not local
 */
int is_local(struct iphdr *header, list_t *interface_lst, pthread_mutex_t *m) {
   pthread_mutex_lock(m);
   for (list_link_t *link = interface_lst->l_next;
        link != interface_lst;
        link = link->l_next) {
     interface *current = list_item(link, interface, listnode);
     if (current->local_virt_ip.s_addr == header->daddr) {
        if (!current->enabled) {
           pthread_mutex_unlock(m);
           return -1;
        }
        pthread_mutex_unlock(m);
        return 1;
     }
   }

   pthread_mutex_unlock(m);
   return 0;
}

void print_packet(char *buffer, unsigned int len) {
   // Buffer contains the ASCII bytes, but not the null terminator
   char s[len + 1];
   int i = 0;
   for (; i < len; i++) {
      s[i] = buffer[i];
   }
   s[i] = '\0';
   printf("message: %s\n", s);
}

/*
 * Check the protocol number of the header.  If it's 0, print the packet.
 * Otherwise, find the handler for that protocol and call it.
 */
void handle_local(struct iphdr *header, char *buffer) {
   if (header->protocol == 0) {
      print_packet(buffer + header->ihl * 4, header->tot_len - sizeof(struct iphdr));
   } else {
      pthread_mutex_lock(&handlers_mutex);
      void *result = htable_get(&handlers, header->protocol);
      pthread_mutex_unlock(&handlers_mutex);

      if (result == NULL) {
         fprintf(stderr, "error: no handler for protocol %u\n", header->protocol);
         return;
      }

      handler_t h = (handler_t)result;
      (*h)(buffer);
   }
}

/*
 * Use the forwarding table to find the next hop, using the destination address
 * from the header.  Then, send the packet on that interface.
 */
void forward(struct iphdr *header, char *buffer, size_t size) {
   printf("forwarding...\n");
   struct in_addr header_dest;
   header_dest.s_addr = header->daddr;
   interface *next_hop = get_interface(&header_dest);
   if (next_hop == NULL) {
      fprintf(stderr, "error: don't know any next hop for that address\n");
      return;
   }

   // Put fields back into network byte order
   header->saddr = ntohl(header->saddr);
   header->daddr = ntohl(header->daddr);
   header->tot_len = ntohs(header->tot_len);

   // New checksum
   header->check = 0;
   uint16_t new_checksum = ip_sum((char *)header, sizeof(struct iphdr));
   header->check = new_checksum;

   link_send_packet(buffer, size, next_hop);
}

/*
 * Register the argument handler for the argument protocol number.
 */
void net_register_handler(uint8_t protocol_num, handler_t handler) {
   pthread_mutex_lock(&handlers_mutex);
   htable_put(&handlers, protocol_num, handler);
   pthread_mutex_unlock(&handlers_mutex);
}

int checksum_matches(struct iphdr *header, char *buffer) {
   return 1;
}

void rip_stub(char *packet) {
   struct iphdr header;
   memcpy(&header, packet, sizeof(struct iphdr));

   size_t size = header.tot_len - sizeof(struct iphdr);
   struct in_addr *src = (struct in_addr *)malloc(sizeof(struct in_addr));
   src->s_addr = header.saddr;

   int status = handle_rip_packet(packet + sizeof(struct iphdr), size, src);
   if (status == -1) {
      fprintf(stderr, "warning: error handling rip packet\n");
   }

   free(src);
}

/*
 * Entry point for forwarding thread.  Argument is a pointer to an nlayer_args
 * struct.
 */
void *network_layer_init(void *arg) {
   nlayer_args *a = (nlayer_args *)arg;
   list_t *interface_lst = a->interface_lst;
   pthread_mutex_t *if_mutex = a->if_mutex;

   pthread_mutex_init(&handlers_mutex, NULL);

   pthread_mutex_lock(&handlers_mutex);
   htable_init(&handlers, 10); // TODO: cap 10?
   pthread_mutex_unlock(&handlers_mutex);


   // Register a handler for RIP packets
   handler_t rip_handler = &rip_stub;
   net_register_handler(200, rip_handler);

   while (1) {
      char buffer[MAX_PACKET_SIZE];
      int size = get_packet(buffer); 
      if (size == -1) {
         fprintf(stderr, "discarding IP packet \n");
         continue;
      }

      if (size == 0) {
         // select timeout hit, just try again
         continue;
      }

      struct iphdr *header = (struct iphdr *)buffer;

      // Compare checksum
      uint16_t checksum = ip_sum(buffer, sizeof(struct iphdr));
      if (checksum != 0) {
         printf("checksum not zero, discarding ip packet\n");
         continue;
      }

      // Change fields from network to host byte order
      header->saddr = ntohl(header->saddr);
      header->daddr = ntohl(header->daddr);
      header->tot_len = ntohs(header->tot_len);
      header->check = ntohs(header->check);

      //print_ip_header(header);

      uint8_t ttl = header->ttl;
      if (ttl <= 0) {
         fprintf(stderr, "discarding IP packet: ttl hit zero\n");
         continue;
      } else {
         header->ttl = header->ttl - 1;
      }

      int location = is_local(header, interface_lst, if_mutex);
      if (location == 1) {
         handle_local(header, buffer);
      } else if (location == 0) {
         forward(header, buffer, size);
      }
   }
}

void print_ip_header(struct iphdr *header) {
   printf("IP packet header: \n");
   printf("    ihl = %u\n", header->ihl);
   printf("    version = %u\n", header->version);
   printf("    tos = %u\n", header->tos);
   printf("    tot_len = %u\n", header->tot_len);
   printf("    id = %u\n", header->id);
   printf("    frag_off = %u\n", header->frag_off);
   printf("    ttl = %u\n", header->ttl);
   printf("    protocol = %u\n", header->protocol);
   printf("    check = %u\n", header->check);
   printf("    saddr = %u\n", header->saddr);
   printf("    daddr = %u\n", header->daddr);
}

/*
 * Given the address of some payload, and the size of that payload, creates
 * an IP header using the argument protocol, and sends it using the UDP socket
 * and remote address from the argument interface.
 */
int send_packet(
      char *payload, 
      size_t size, 
      int protocol, 
      interface *to, 
      uint32_t saddr, 
      uint32_t daddr) {
  
   if (to == NULL) {
     printf("error: could not send the packet to the destination\n");
     return -1;
   }

   if (!to->enabled) {
      printf("can't send to this interface, it's down\n");
      // Can't send to a down interface
      return -1;
   }

   struct iphdr *header = (struct iphdr *)malloc(sizeof(struct iphdr));
   header->ihl = 5;
   header->version = 4;
   header->tos = 0;
   header->tot_len = sizeof(struct iphdr) + size;
   header->id = 0;
   header->frag_off = 0;
   header->ttl = 16;
   header->protocol = protocol;
   header->check = 0;
   header->saddr = saddr;
   header->daddr = daddr;

   // To network byte order
   header->saddr = htonl(header->saddr);
   header->daddr = htonl(header->daddr);
   header->tot_len = htons(header->tot_len);

   // Compute and store checksum
   uint16_t checksum = ip_sum((char *)header, sizeof(struct iphdr));
   header->check = checksum;

   char *packet = (char *)malloc(header->tot_len);
   memcpy(packet, header, sizeof(struct iphdr));
   memcpy(packet + sizeof(struct iphdr), payload, size);

   link_send_packet(packet, ntohs(header->tot_len), to);

   free(header);
   free(packet);

   return 0;
}
