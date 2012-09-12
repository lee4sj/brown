#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>
#include <errno.h>

#include "linklayer.h"
#include "interface.h"
#include "parselinks.h"

#include "util/dbg.h"
#include "netlayer.h"
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "util/list.h"

/* BEGIN SOLUTION */
//these are header files not given to the students:
#include "route.h"
#include "iputil.h"

//they don't create faulty nodes:
double   g_dropRate = 0.0;
uint32_t g_bandwidth = 0;

int addr_from_host(const char *host, struct sockaddr_in *addr) {
  struct addrinfo hints; 
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_DGRAM;

  struct addrinfo* servinfo;
  int ret = getaddrinfo(host, NULL, &hints, &servinfo);
  if (ret != 0)
    return -1;
  memcpy(addr, servinfo->ai_addr, sizeof(struct sockaddr_in));
  freeaddrinfo(servinfo);
  return 0;
}

interface_t **make_interfaces_from_links(list_t *links) {
  int num_links = 0;
  int i;
  lnxline_t *line;
  
  list_iterate_begin(links, line, lnxline_t, listnode) {
    num_links += 1;
  } list_iterate_end();

  interface_t **interfaces =
    (interface_t **)malloc((num_links+1)*sizeof(interface_t *));
  if (interfaces == NULL) {
    fprintf(stderr, "fatal; out of memory\n");
    exit(1);
  }

  i = 0;
  list_iterate_begin(links, line, lnxline_t, listnode) {
    interfaces[i] = (interface_t *)malloc(sizeof(interface_t));
    if (interfaces[i] == NULL) {
      fprintf(stderr, "out of memory\n");
      exit(1);
    }

    interfaces[i]->sock = -1;
    // set local_phys_addr
    int ret = addr_from_host(line->local_phys_host,
                             &interfaces[i]->local_phys_addr);
    if (ret == -1) {
      dbg(DBG_ERROR, "could not resolve hostname '%s', attempting others...\n",
          line->local_phys_host);
      continue;
    }
    interfaces[i]->local_phys_addr.sin_port = htons(line->local_phys_port);
    // set local_virt_ip
    memcpy(&interfaces[i]->local_virt_ip, &line->local_virt_ip,
           sizeof(interfaces[i]->local_virt_ip));
    // set remote_phys_addr
    ret = addr_from_host(line->remote_phys_host,
                         &interfaces[i]->remote_phys_addr);
    if (ret == -1) {
      dbg(DBG_ERROR, "could not resolve hostname '%s', attempting others...\n",
          line->local_phys_host);
      continue;
    }
    interfaces[i]->remote_phys_addr.sin_port = htons(line->
                                                     remote_phys_port);
    // set remote_virt_ip
    memcpy(&interfaces[i]->remote_virt_ip, &line->remote_virt_ip,
           sizeof(interfaces[i]->remote_virt_ip));
    dbg(DBG_LINK, "Bringing up %s", inet_ntoa_host(interfaces[i]->local_virt_ip));
    dbg(DBG_LINK, "->%s\n", inet_ntoa_host(interfaces[i]->remote_virt_ip));

    //set up faultiness.
    interfaces[i]->droprate = g_dropRate;
    interfaces[i]->bandwidth = g_bandwidth;
    // bring up interface
    ret = link_up(interfaces[i]);
    if (ret == -1) {
      dbg(DBG_ERROR, "could not initialize interface %i\n", i);
      interfaces[i]->sock = -1;
    }
    i += 1;
  } list_iterate_end();
  interfaces[num_links] = NULL;

  return interfaces;
}

/* END SOLUTION */

int driver(interface_t **interfaces);

int main(int argc, char** argv) {
  if (argc < 2) {
    /* BEGIN SOLUTION */
    fprintf(stderr, "Usage: %s linkfile [droprate] [maxbandwidth] [seed]\n", 
            argv[0]);
    fprintf(stderr, "droprate is a percent. bandwidth is in bytes/sec.\n");   
    fprintf(stderr, "seed is a number to use for the random generator. By default, the\n\
current time is used.\n\n");
    fprintf(stderr, "This node will drop a packet it sends with the %% chance given \n\
as the droprate. It will drop all packets it sends if the total number of\n\
bytes sent in the past second has exceeded the bandwidth. It will drop\n\
any packet it receives if it has received more than the max bandwidth\n\
in the past second.  \n\
\n\
Note: The node just resets the amount of bytes it has sent and\n\
received once per second, so it's not the most accurate.\n");
    return 1;
    /* END SOLUTION */
    fprintf(stderr, "Usage: %s linkfile\n", argv[0]);
    return 1;
  }

  char* linkfile = argv[1];
  list_t* links = parse_links(linkfile);
  if (links == NULL)
    return 1;

  interface_t **interfaces = NULL;
  //TODO: initialize interfaces
  
  dbg_add_mode("error");

  dbg(DBG_LINK, "Link-layer debugging messages enabled.\n");
  dbg(DBG_NET, "Net-layer debugging messages enabled.\n");
  dbg(DBG_ROUTE, "Routing debugging messages enabled.\n");
  /* BEGIN SOLUTION */ 

  if (argc >= 3) {
    g_dropRate = atof(argv[2]);
    printf("This node is faulty! It has a drop rate of %f", g_dropRate);
  }
  if (argc >= 4) {
    g_bandwidth = atoi(argv[3]);
    printf(", and a bandwidth of %d bytes/sec, both in and out", g_bandwidth);
  }
  printf("\n");

  int seed = time(NULL);
  if (argc >= 5) {
    seed = atoi(argv[4]);
  }
  srand48(seed);

  interfaces = make_interfaces_from_links(links);
  if (interfaces == NULL) {
    dbg(DBG_ERROR, "error making interfaces\n");
    return 1; 
  }
  /* END SOLUTION */

  return driver(interfaces);
}

/* BEGIN SOLUTION */
void default_handler(interface_t *interface, struct ip *packet)
{
  (void)interface; //squish compiler warning
  printf("default_handler: received a packet\n");
  ip_packet_print(packet, 2);
}

extern pthread_mutex_t g_routeMutex;
extern htable_t *g_routeTable;
/* END SOLUTION */
int driver(interface_t **interfaces) {
/* BEGIN SOLUTION */
  int i = 0, res;
  char buf[356];

  struct ip *packet;

  routeentry_t *entry;
    
  struct in_addr destaddr, emptyaddr;
  emptyaddr.s_addr = 0;
  int hashkey;
    
  //start IP and routing
  net_init(interfaces, default_handler);
  route_init(interfaces);    

  printf("Node all set [ CTRL-D / CTRL-C to exit ]\n");
  printf("Commands:\n");
  printf("  1 - write.\n");
  printf("  2 - print routing table.\n");
  printf("  3 - print interfaces and upstates.\n");
  printf("  4 - tear down an interface.\n");
  printf("  5 - bring up an interface.\n");
  printf("  9 - print this menu.\n");

  while (1) {
    printf( "Command: " );

    fflush(stdout);
    fgets(buf, 250, stdin);
    res = sscanf(buf, "%d", &i);
    if (res != 1) continue;

    switch( i ) {
    case 1: {
      printf( "Destination (ip): " );
      fflush(stdout);
      memset(buf,0,80);
      read(STDIN_FILENO, buf, 80);
      buf[79] = 0;
      res = inet_aton(buf, &destaddr);
      if (res == 0) {
        printf("Invalid argument.\n");
        continue;
      }
      destaddr.s_addr = ntohl(destaddr.s_addr);
	    
      printf( "Message: " );
      fgets(buf, 350, stdin);
      packet = ip_packet_construct(emptyaddr, destaddr, 0, ROUTE_INFINITY, 0, buf, strlen(buf));
      res = net_send(packet);
      ip_packet_destruct(packet);
            
      if( res > 0 )
        printf( "send success\n" );
      else
        printf( "send error: %s\n", strerror(-res));
            
      break;
    }
            
    case 2: {
      //print routing table
      pthread_mutex_lock(&g_routeMutex);
            
      printf("Routing table:\n");
      struct in_addr tmp;
      htable_iterate_begin(g_routeTable, hashkey, entry, routeentry_t) {
        tmp.s_addr = hashkey;
        printf("  Route to %s with cost %d, ", inet_ntoa_host(tmp), entry->cost);
        printf("through %s\n", 
               (entry->interface == NULL ? "self" : 
                inet_ntoa_host(entry->interface->remote_virt_ip)));
      } htable_iterate_end();

      pthread_mutex_unlock(&g_routeMutex);
      break;
    }

    case 3: {
      //print interfaces
      printf("Interfaces:\n");            
      //print interface states
      for (i=0; interfaces[i]; ++i) {
        printf("  %d: %s", i, inet_ntoa_host(interfaces[i]->local_virt_ip));
        printf("->%s", inet_ntoa_host(interfaces[i]->remote_virt_ip));
        printf(", %s\n", interfaces[i]->sock==-1 ? "DOWN" : "UP");
      }
      break;
    }  

    case 4: {
      int inum;
      printf( "Interface to tear down: " );
      fflush(stdout);
      memset(buf,0,80);
      read(STDIN_FILENO, buf, 80);
      buf[79] = 0;
      res = sscanf(buf, "%d", &inum);      
      if (res != 1) {
        printf("Invalid argument.\n");
        continue;
      }
      for (i=0; interfaces[i]; ++i);
      if (inum < 0 || inum >= i) {
        printf("No such interface.\n");
        continue;
      }
      if (interfaces[inum]->sock == -1) {
        printf("interface %d already down.\n", inum);
        continue;
      }
      res = link_down(interfaces[inum]);
      if (res < 0)
        perror("link_down failed: ");
      break;
    }
        
    case 5: {
      int inum;
      printf( "Interface to bring up: " );
      fflush(stdout);
      memset(buf,0,80);
      read(STDIN_FILENO, buf, 80);
      buf[79] = 0;
      res = sscanf(buf, "%d", &inum);      
      if (res != 1) {
        printf("Invalid argument.\n");
        continue;
      }
      for (i=0; interfaces[i]; ++i);
      if (inum < 0 || inum >= i) {
        printf("No such interface.\n");
        continue;
      }
      if (interfaces[inum]->sock != -1) {
        printf("interface already up\n");
        continue;
      }
      res = link_up(interfaces[inum]);
      if (res < 0) {
        perror("link_up failed: ");
        continue;
      }
      pthread_t recvThreadID;
      printf("Creating receive thread on interface from %s",
             inet_ntoa_host(interfaces[inum]->local_virt_ip));
      printf(" to %s\n", inet_ntoa_host(interfaces[inum]->remote_virt_ip));
      pthread_create(&recvThreadID, NULL, net_recv_thread, (void *)interfaces[inum]);
      trigger_update();
      break;
    }
	  
    case 9: {
      printf("Commands:\n");
      printf("  1 - write.\n");
      printf("  2 - print routing table.\n");
      printf("  3 - print interfaces and upstates.\n");
      printf("  4 - tear down an interface.\n");
      printf("  5 - bring up an interface.\n");
      printf("  9 - print this menu.\n");
      break;
    }
    }
  }
    
  route_exit();
  net_exit();
/* END SOLUTION */
  return 0;
}
