#include <memory.h>
#include <limits.h>
#include <pthread.h>

#include <arpa/inet.h>

#include "iputil.h"
#include "util/ipsum.h"
#include "util/dbg.h"

//global ID
static uint16_t g_curId = 0;
static pthread_mutex_t g_idMutex = PTHREAD_MUTEX_INITIALIZER;

#define FRAGINFO_DNF(fragInfo)        ((0x8000 & (fragInfo)) >> 15)
#define FRAGINFO_MF(fragInfo)         ((0x4000 & (fragInfo)) >> 14)
#define FRAGINFO_FRAGOFFSET(fragInfo) ((0x3FFF & (fragInfo)))

#define FRAGINFO_SETDNF(fragInfo, DNF)         (DNF     ? (fragInfo |= 0x8000) : (fragInfo &= 0x7FFF))
#define FRAGINFO_SETMF(fragInfo, fragged)      {fragInfo = ((fragged) ? ((fragInfo) | 0x4000) : ((fragInfo) & 0xBFFF));}
#define FRAGINFO_SETOFFSET(fragInfo, offset)   {fragInfo &= 0xC000; fragInfo |= ((offset) & 0x3FFF);}

//note: only valid if the packet fields are in nbo.
int ip_calc_checksum(struct ip *packet)
{
  int oldsum = packet->ip_sum;
  
  packet->ip_sum = 0;
  packet->ip_sum = ip_sum((char *)packet, sizeof(struct ip));
  
  return oldsum == packet->ip_sum;
}

struct ip *ip_packet_construct(
  struct in_addr src, struct in_addr dest,
  int DNF, uint8_t timeToLive, uint8_t protocol, 
  void *data, uint16_t datalen)
{
  struct ip *packet;

  if (datalen > USHRT_MAX - sizeof(struct ip))
  {
    dbg(DBG_NET, "ip_packet_construct: Can't construct packet. length %d is too large.\n", datalen);
    return NULL;
  }
    
  packet = (struct ip *)malloc(sizeof(struct ip) + datalen);

  memcpy(packet+1, data, datalen); //due to pointer math, packet+1, not packet+sizeof(struct ip)
    
  packet->ip_v = 4;
  packet->ip_hl = 5; //5, because we never have any options
  packet->ip_tos = 0;

  //total length
  packet->ip_len = sizeof(struct ip) + datalen;

  pthread_mutex_lock(&g_idMutex);
  packet->ip_id = g_curId;
  ++g_curId;
  pthread_mutex_unlock(&g_idMutex);

  packet->ip_off = 0;
  FRAGINFO_SETDNF(packet->ip_off, DNF);

  packet->ip_ttl = timeToLive;
  packet->ip_p = protocol;
    
  packet->ip_src = src;
  packet->ip_dst = dest;
    
  //ip_calc_checksum(packet);      
  packet->ip_sum = 0; 
    
  return packet;
}

void ip_packet_destruct(struct ip *packet)
{
  if (packet == NULL) return;
  free(packet);
}

//convert a packet's header to network byte order
void ip_packet_hton(struct ip *packet)
{
  packet->ip_len = htons(packet->ip_len);
  packet->ip_id = htons(packet->ip_id);
  packet->ip_off = htons(packet->ip_off);
    
  packet->ip_src.s_addr = htonl(packet->ip_src.s_addr);
  packet->ip_dst.s_addr = htonl(packet->ip_dst.s_addr );
    
  packet->ip_sum = htons(packet->ip_sum);
}
//convert a packet's header to host order
void ip_packet_ntoh(struct ip *packet)
{
  packet->ip_len = ntohs(packet->ip_len);
  packet->ip_id = ntohs(packet->ip_id);
  packet->ip_off = ntohs(packet->ip_off);
    
  packet->ip_src.s_addr = ntohl(packet->ip_src.s_addr);
  packet->ip_dst.s_addr = ntohl(packet->ip_dst.s_addr );
    
  packet->ip_sum = ntohs(packet->ip_sum);
}

void ip_packet_print(struct ip *packet, int printData)
{
  static char maxstr[65535];
  printf("src:         %s\n", inet_ntoa_host(packet->ip_src));
  printf("dst:         %s\n", inet_ntoa_host(packet->ip_dst));
  printf("totalLength: %d\n"
         "DNF:         %d\n"
         "MF:          %d\n"
         "fragOffset:  %d\n"
         "id:          %d\n"
         "TTL:         %d\n"
         "protocol:    %d\n"
         "checksum:    %x\n",
         packet->ip_len,
         FRAGINFO_DNF(packet->ip_off),
         FRAGINFO_MF(packet->ip_off),
         FRAGINFO_FRAGOFFSET(packet->ip_off),
         packet->ip_id,
         packet->ip_ttl,
         packet->ip_p,
         packet->ip_sum);
  if (printData) {
    printf("data:\n");
    if (printData == 1) {
      for (unsigned int i=0; i < packet->ip_len - sizeof(struct ip); ++i)
      {
        printf("%02x ", ((uint8_t *)(packet+1))[i]);
        if (i%20==0) printf("\n");
      }
    }
    else if (printData == 2) {
      memcpy(maxstr, packet+1, packet->ip_len - sizeof(struct ip));
      maxstr[packet->ip_len - sizeof(struct ip)] = '\0';
      printf("%s\n", maxstr);
    }
  }
}             

char *inet_ntoa_host(struct in_addr a)
{
  struct in_addr tmp;
  tmp.s_addr = htonl(a.s_addr);
  return inet_ntoa(tmp);
}
