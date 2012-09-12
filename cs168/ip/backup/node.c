#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <inttypes.h>
#include <inttypes.h>
#include <arpa/inet.h>
#include <pthread.h>

#include "parselinks.h"

#include "rip.h"
#include "network_layer.h"
#include "link_layer.h"


// From the K&R C book
void reverse(char s[])
 {
     int i, j;
     char c;
 
     for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
         c = s[i];
         s[i] = s[j];
         s[j] = c;
     }
 }

// Also from the K&R C book
void itoa(int n, char s[])
{
    int i, sign;

    if ((sign = n) < 0) 
        n = -n;         
    i = 0;
    do {      
        s[i++] = n % 10 + '0';  
    } while ((n /= 10) > 0);    
    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';
    reverse(s);
}

int int_strlen(int n) {
   int count = 0;

   do {
      count += 1;
   } while ((n /= 10) > 0);

   return count + 1;
}

void print_interface_info(list_t *interface_lst, pthread_mutex_t *m) {
   pthread_mutex_lock(m);
   for (list_link_t *link = interface_lst->l_next;
         link != interface_lst;
         link = link->l_next) {
      interface *current = list_item(link, interface, listnode);
      char locstr[INET6_ADDRSTRLEN], remstr[INET6_ADDRSTRLEN];

      uint32_t lvip = htonl(current->local_virt_ip.s_addr);
      uint32_t rvip = htonl(current->remote_virt_ip.s_addr);
      
      inet_ntop(AF_INET, &(lvip), locstr, sizeof(locstr));
      inet_ntop(AF_INET, &(rvip), remstr, sizeof(remstr));
      printf("interface %d: enabled = %d, local IP = %s, remote IP = %s\n",
            current->id, current->enabled, locstr, remstr);
   }
   pthread_mutex_unlock(m);
}

void handle_updown_command(int iface_num, 
      int new_status, 
      list_t *interface_lst, 
      pthread_mutex_t *m) {
   pthread_mutex_lock(m);
   for (list_link_t *link = interface_lst->l_next;
         link != interface_lst;
         link = link->l_next) {
      interface *current = list_item(link, interface, listnode);
      if (current->id == iface_num) {
         current->enabled = new_status;
	 
	 if(new_status == 0)
	   interface_down(iface_num);
	 else
	   interface_up(iface_num);
	 
         pthread_mutex_unlock(m);
	 
         return;
      }
   }
   pthread_mutex_unlock(m);
   fprintf(stderr, "error: invalid interface number\n"); 
}

/*
 * Call into the network layer to send a packet on the interface designated
 * by vip, using protocol proto, with payload s.
 */
void handle_send_command(char *vip,
			 int proto,  
			 char *s, 
			 list_t *interface_lst,
			 pthread_mutex_t *m) {
  
   in_addr_t target_vip = ntohl(inet_addr(vip));
   interface *target_iface = NULL;
   
   pthread_mutex_lock(m);
   for (list_link_t *link = interface_lst->l_next;
	   link != interface_lst;
	   link = link->l_next) {


      interface *current = list_item(link, interface, listnode);
      if (current->local_virt_ip.s_addr == target_vip) {
         pthread_mutex_unlock(m);
         printf("message: %s\n", s);
         return;
      }
   }
   
   pthread_mutex_unlock(m);
   struct in_addr dest_addr;
   dest_addr.s_addr = target_vip;
   target_iface = get_interface(&dest_addr);
   
   if(target_iface == NULL)
     printf("send error: no route or host.\n");
   
   else
     send_packet(
		 s, 
		 strlen(s), 
		 proto, 
		 target_iface, 
		 target_iface->local_virt_ip.s_addr,
		 target_vip);   
}

/*
 * Reads the .lnx file and returns a list of interfaces.
 */
list_t *initialize_interfaces(char *lnx_path) {
   list_t *interface_lst;
   list_t *lnxfile_lst = parse_links(lnx_path);

   interface_lst = (list_t *)malloc(sizeof(list_t));
   list_init(interface_lst);

   if (lnxfile_lst == NULL) {
      fprintf(stderr, "error parsing .lnx file\n");
      exit(1);
   }

   list_link_t *link;
   int next_id = 0;
   for (link = lnxfile_lst->l_next; link != lnxfile_lst; link = link->l_next) {
      lnxline_t *current = list_item(link, lnxline_t, listnode);

      int sockfd;

      // Turn UDP port unsigned ints into strings
      int l = int_strlen(current->local_phys_port);
      char *lphys_str = (char *)malloc(sizeof(char) * l);
      itoa(current->local_phys_port, lphys_str);
      //printf("local phys addr: %s\n", current->local_phys_host);
      //printf("local phys port: %s\n", lphys_str);

      l = int_strlen(current->remote_phys_port);
      char *rphys_str = (char *)malloc(sizeof(char) * l);
      itoa(current->remote_phys_port, rphys_str);
      //printf("remote phys addr: %s\n", current->remote_phys_host);
      //printf("remote phys port: %s\n", rphys_str);

      // Get addrinfo struct for remote address + port 
      // (to store in interface struct as "to")
      int status;
      struct addrinfo r_hints;
      struct addrinfo *r_servinfo;

      memset(&r_hints, 0, sizeof(r_hints));
      r_hints.ai_family = AF_INET;
      r_hints.ai_socktype = SOCK_DGRAM;
      status = getaddrinfo(current->remote_phys_host,
            lphys_str,
            //NULL,
            &r_hints,
            &r_servinfo);      
      struct sockaddr_in *temp = (struct sockaddr_in *)r_servinfo->ai_addr;
      temp->sin_port = htons(current->remote_phys_port);
      
      if (status != 0) {
         fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
         exit(1);
      }
      
      // TODO: when can i call freeaddrinfo on r_servinfo?

      // Create socket
      struct addrinfo hints;
      struct addrinfo *servinfo, *p;  // will point to the results

      memset(&hints, 0, sizeof(hints)); // make sure the struct is empty
      hints.ai_family = AF_INET;     // IPV4
      hints.ai_socktype = SOCK_DGRAM; // UDP socket
      if ((status = getaddrinfo(current->local_phys_host, 
                  //lphys_str,
                  NULL,
                  &hints, 
                  &servinfo)) != 0) {
         fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
         exit(1);
      }

      for (p = servinfo; p != NULL; p = p->ai_next) {
      	temp = (struct sockaddr_in *)p->ai_addr;
      	temp->sin_port = htons(current->local_phys_port);

         sockfd = socket(p->ai_family,
               p->ai_socktype,
               p->ai_protocol);

         if (sockfd == -1) {
            continue;
         }

         if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("listener: bind");
            continue;
         }
 
         break;
      }

      if (p == NULL) {
         fprintf(stderr, "error binding socket\n");
         exit(1);
      }

      interface *current_if = (interface *)malloc(sizeof(interface));
      current_if->id = next_id;
      current_if->enabled = 1;
      current_if->socketfd = sockfd;
      current_if->to = r_servinfo->ai_addr;
      current_if->tolen = r_servinfo->ai_addrlen;
      current_if->local_virt_ip = current->local_virt_ip;
      current_if->remote_virt_ip = current->remote_virt_ip;
      list_insert_tail(interface_lst, &current_if->listnode);

      
      freeaddrinfo(servinfo); // free the linked-list
      next_id += 1;
   }

   return interface_lst;
}

int main(int argc, char **argv) {
   if (argc != 2) {
      fprintf(stderr, "node takes one argument: path to a .lnx file\n");
      exit(1);
   }

   list_t *interface_lst = initialize_interfaces(argv[1]);
   pthread_mutex_t if_mutex;
   pthread_mutex_init(&if_mutex, NULL);

   printf("%s: node running...\n", argv[1]);

   // Give reference to interface list and its mutex to link layer
   link_layer_init(interface_lst, &if_mutex);

   // start forwarding thread
   nlayer_args a;
   a.interface_lst = interface_lst;
   a.if_mutex = &if_mutex;
   void *arg = (void *)(&a);

   pthread_t nl_thread;
   int result = pthread_create(&nl_thread, NULL, network_layer_init, arg);
   if (result != 0) {
     fprintf(stderr, "pthread_create error: %s\n", gai_strerror(result));
     exit(1);
   }


   pthread_t routing_thread;
   arg = (void *)interface_lst;
   result = pthread_create(&routing_thread, NULL, rip_loop, arg);
   if (result != 0) {
     fprintf(stderr, "pthread_create error: %s\n", gai_strerror(result));
     exit(1);
   }

   // Read keyboard commands
   int default_bufsize = 128;
   char *buf = (char *)malloc(default_bufsize + 1);
   char *copy, *running, *token, *delimiters = " \n";
   int bytes_read;
   while (1) {
      printf(">");
      bytes_read = getline(&buf, &default_bufsize, stdin);
      if (bytes_read == -1) {
         printf("command line error\n");
         pthread_exit(NULL);
      } 

      if (!strcmp(buf, "interfaces\n")) {
         print_interface_info(interface_lst, &if_mutex);
         continue;
      } 

      if (!strcmp(buf, "routes\n")) {
         print_routes_info();
         continue;
      }

      copy = (char *)malloc(strlen(buf) + 1);
      strcpy(copy, buf);
      running = copy;
      running[strcspn(running, "\n")] = '\0';
      token = strsep(&running, delimiters);

      if (!strcmp(token, "down")) {
         // down command
         token = strsep(&running, delimiters);
         if (token == NULL) {
            printf("badly formed down command\n");
         } else {
            char *end;
            int n = strtol(token, &end, 10);
            if (end == token || *end != '\0') {
               printf("argument to down command must be an integer\n");
            } else {
               handle_updown_command(n, 0, interface_lst, &if_mutex);
            }
         }
         continue;
      } 

      if (!strcmp(token, "up")) {
         // up command
         token = strsep(&running, delimiters);
         if (token == NULL) {
            printf("badly formed up command\n");
         } else {
            char *end;
            int n = strtol(token, &end, 10);
            if (end == token || *end != '\0') {
               printf("argument to up command must be an integer\n");
            } else {
               //handle_up_command(n);
               handle_updown_command(n, 1, interface_lst, &if_mutex);
            }
         }
         continue;
      }

      if (!strcmp(token, "send")) {
         // send command
         char *send_args[3];
         int i, bad_command = 0;
         for (i = 0; i < 3; i++) {
            token = strsep(&running, delimiters);
            if (token == NULL) {
               printf("badly formed send command\n");
               bad_command = 1;
               break;
            }
            send_args[i] = token;
         }
         if (!bad_command) {
            char *end;
            int proto = strtol(send_args[1], &end, 10);
            if (end == send_args[1] || *end != '\0') {
               printf("protocol argument to send must be an integer\n");
            } else {
               handle_send_command(
                     send_args[0], proto, send_args[2], interface_lst, &if_mutex);
            }
         }
         continue;
      }

      // If we made it here, it's not a command
      printf("unrecognized command\n");

      free(copy);
   }

   return 0;
}
