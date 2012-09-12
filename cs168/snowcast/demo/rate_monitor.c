/*
 *  Acct: csaftoiu
 *  Desc: Assignment 1: Snowcast
 *        UDP Client.
 */
//syntax: listener portno
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFERLEN 1024

int main(int argc, char *argv[])
{
    char buffer[BUFFERLEN];
    int res;
    struct timeval lasttime, curtime, timeout;
    double secelapsed, datarate=0.0;
    long long bytesread = 0;

    fd_set stdinset;
    int fdsready;

    res = gettimeofday(&lasttime, NULL);
    if (res < 0) { perror("gettimeofday"); return -1; }

    while(1)
    {
      FD_ZERO(&stdinset);
      FD_SET(STDIN_FILENO, &stdinset);
      timeout.tv_sec = 0;
      timeout.tv_usec = 500000;
      fdsready = select(STDIN_FILENO+1, &stdinset, NULL, NULL, &timeout);
      if (fdsready < 0) { perror("select"); return -1; }
      if (fdsready > 0)
      {
          res = read(STDIN_FILENO, buffer, BUFFERLEN);
          if (res == 0) {
            fprintf(stderr, "stdin has closed.\n");
            return 0;
          }
          if (res < 0) {
            perror("read"); return -1;
          }
          //fprintf(stderr, "res=%d\n", res);
          write(STDOUT_FILENO, buffer, res);

          bytesread += res;
      }

      res = gettimeofday(&curtime, NULL);
      if (res < 0) { perror("gettimeofday"); return 0; }
      secelapsed = curtime.tv_sec - lasttime.tv_sec + 
                   (curtime.tv_usec - lasttime.tv_usec) / 1000000.0;
      if (secelapsed >= 0.5) {
          datarate = bytesread/1024.0/secelapsed; //kilobytes / seocnd
          fprintf(stderr, "\r%8.4lf kB/s", datarate);
          lasttime = curtime;
          bytesread = 0;
      }
    }

    return 0;
}
