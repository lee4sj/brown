#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <arpa/inet.h>
#include <time.h>
#include <errno.h>

struct timespec testa(struct timespec a){
  a.tv_sec = a.tv_sec * 2;
  return a;
}

int main(int argc, char* argv[]){
  
  char *ip1 = "128.140.80.0";
  char *ip2 = "128.140.127.255";

  in_addr_t a1 = htonl(inet_addr(ip1));
  in_addr_t a2 = htonl(inet_addr(ip2));

  printf("ip1: %u\n", a1);
  printf("ip2: %u\n", a2);
  printf("ip2 - ip1: %u\n", (a2 -a1));


  int a = 10;
  if(a)
    printf("that is right\n");

  if( 10 < 332 < 199)
    printf("%d\n", 2*4);


  /*  printf("%d\n", sizeof(suseconds_t));
  suseconds_t k = 0xffffffff;
  printf("%u\n", k);
  */
  struct timespec curr;

  clock_gettime(CLOCK_REALTIME, &curr);

  struct timespec timestamp;

  
  memcpy(&timestamp, &curr, sizeof(struct timespec));

  printf("%d\n", timestamp);

  clock_gettime(CLOCK_REALTIME, &curr);

  printf("%u: %u\n", testa(timestamp).tv_sec, timestamp.tv_sec);


  if(121)
    printf("YES\n");


  char temp1[100];
  char temp2[100];
  temp2[10] = 'h';
  temp2[11] = 'd';

  memcpy(temp1, &temp2[10], 2);
  printf("%s\n", temp1);


  long k = 10;
  double v = (double)k;

  printf("%f\n", v);
}
