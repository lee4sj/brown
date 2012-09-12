#include <stdio.h>
#include <inttypes.h>
#include <inttypes.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <pthread.h>
#include "util/list.h"
#include "rip.h"
#include "network_layer.h"


// Global variables to hold interface list and mutex
list_t *interface_lst;
pthread_mutex_t *if_mutex;

/*
 * Entry point for link layer.  Arguments are a pointer to the list
 * of interfaces, and a pointer to the mutex protecting the list.
 */
void link_layer_init(list_t *arg1, pthread_mutex_t *arg2) {
   interface_lst = arg1;
   if_mutex = arg2;
   pthread_mutex_init(if_mutex, NULL);
}

/*
 * Use recvfrom to read an IP packet into buffer, returning the size of the
 * packet
 */
int read_packet(int sockfd, char *buffer) {
   char *orig_buffer = buffer;
   unsigned int bytes_read = 0, header_size = sizeof(struct iphdr);
   struct sockaddr sender;
   socklen_t from_len = 0;
   while (bytes_read < header_size) {
      unsigned int last_read = recvfrom(
					sockfd, 
					buffer, 
					MAX_PACKET_SIZE - bytes_read,
					0,
					&sender,
					&from_len);
      if (last_read == -1) {
         fprintf(stderr, "recvfrom error\n");
         pthread_exit(NULL);
      } else {
         bytes_read += last_read;
         buffer += last_read;
      }
   }

   struct iphdr *header = (struct iphdr *)orig_buffer;

   while (bytes_read < ntohs(header->tot_len)) {
      unsigned int last_read = recvfrom(sockfd, 
					buffer, 
					MAX_PACKET_SIZE - bytes_read,
					0,
					&sender,
					&from_len);
      if (last_read == -1) {
         fprintf(stderr, "recvfrom error\n");
         pthread_exit(NULL);
      } else {
         bytes_read += last_read;
         buffer += last_read;
      }
   }

   struct sockaddr_in psender;
   memcpy(&psender, &sender, sizeof(struct sockaddr));

   return ntohs(header->tot_len);
}

/*
 * Given the address of a 64k buffer, blocks until an IP packet has been recieved
 * on some interface.  Then, writes that packet to the buffer, and returns the
 * size of the packet.
 */
int get_packet(char *buffer) {
   fd_set readfds;
   FD_ZERO(&readfds);
   int max_fd = 0;
   int bytes_read = 0;

   pthread_mutex_lock(if_mutex);
   for (list_link_t *link = interface_lst->l_next;
         link != interface_lst;
         link = link->l_next) {
      interface *current = list_item(link, interface, listnode);

      int currentfd = current->socketfd;
      FD_SET(currentfd, &readfds);
      if (currentfd > max_fd) max_fd = currentfd;
   }
   pthread_mutex_unlock(if_mutex);

   struct timeval timeout;
   timeout.tv_sec = 0;
   timeout.tv_usec = 500000;

   select(max_fd + 1, &readfds, NULL, NULL, &timeout);

   pthread_mutex_lock(if_mutex);
   for (list_link_t *link = interface_lst->l_next;
         link != interface_lst;
         link = link->l_next) {
      interface *current = list_item(link, interface, listnode);

      int currentfd = current->socketfd;
      if (FD_ISSET(currentfd, &readfds)) {
         // TODO: should this unlock after reading packet?
         // What if "down" command while reading from this socket?
         // But I don't want to hold the lock while calling recvfrom
         // and looping until you get a whole packet.
         pthread_mutex_unlock(if_mutex);
         bytes_read = read_packet(currentfd, buffer);
         return bytes_read;
      }
   }

   // Got here if nothing came within the timeout
   pthread_mutex_unlock(if_mutex);

   return 0;
}

/*
 * Given the address of an IP packet with header, and the size of the packet,
 * sends that packet using the UDP socket and remote address from the argument
 * interface.
 */

int link_send_packet(char *packet, size_t size, interface *to) {
   unsigned int bytes_sent = sendto(to->socketfd, 
				    packet, 
				    size, 
				    0, 
				    to->to, 
				    to->tolen);
   if (bytes_sent == -1) return -1;
   packet += bytes_sent;
   while (bytes_sent < size) {
      int last_sent = sendto(to->socketfd, packet, size, 0, to->to, to->tolen);
      if (last_sent == -1) {
         return -1;
      } else {
         bytes_sent += last_sent;
         packet += last_sent;
      }
   }
   return bytes_sent;
}
