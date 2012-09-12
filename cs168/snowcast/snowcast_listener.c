/* 
 * File:   snowcast_listener.c
 * Author: sl136
 *
 * Created on February 3, 2011, 7:17 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdint.h>
#include <strings.h>


#define PACKSIZE 1024


int udpsocket(int port, int addr){
    int fd;
    struct sockaddr_in sin;

    if(port < 1 || port > 0xffff){
        fprintf(stderr, "bad prot number\n");
        exit(1);
    }

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(fd < 0){
        perror("socket");
        exit(1);
    }

    bzero(&sin, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(port);
    sin.sin_addr.s_addr = htonl(addr);
    if(bind(fd, (struct sockadder *) &sin, sizeof(sin)) < 0){
        perror("bind");
        exit(1);
    };

    return fd;
}

/*
 * 
 */
int main(int argc, char** argv) {

    if(argc < 2){
        printf("port number is required!\n");
        exit(1);
    }

    int port = atoi(argv[1]);

    if(port < 1 || port > 0xffff){
        perror("port is invalid. i.e. > 0xffff or < 1 or not a number");
        exit(1);
    }

    int fd;
    char buf[PACKSIZE];
    int received_bytes;

    fd = udpsocket(port, INADDR_ANY);
    printf("udpsocket set!\n");




    //receiving bytes:
    for(;;){
        struct sockaddr_in from;
        socklen_t fromlen = sizeof(from);
        bzero(&from, sizeof(from));

        received_bytes = recvfrom(fd, buf, sizeof(buf), 0, (struct sockaddr*) &from, &fromlen);
        if(received_bytes < 0){
            perror("Failure in receiving packages!");
            exit(1);
        }

        write(1, buf, received_bytes);

    }

    return (EXIT_SUCCESS);
}

























