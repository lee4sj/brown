/* 
 * File:   snowcast_control.c
 * Author: sl136
 *
 * Created on February 4, 2011, 12:36 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdint.h>
#include <strings.h>
#include <netdb.h>

#define TCPPORT 0010

int tcpsocket(char* servername, int serverport){
    int tcpsock = socket(AF_INET, SOCK_STREAM, 0);
    if(tcpsock < 0){
        perror("tcpsock");
        exit(1);
    }

    struct hostent *host = gethostbyname(servername);

    //connect the socket to the server
    struct sockaddr_in serveraddr;
    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(serverport);
    memcpy(&serveraddr.sin_addr, host->h_addr_list[0], host->h_length);
   
    if(connect(tcpsock, (struct sockadder *) &serveraddr, sizeof(serveraddr)) < 0){
        perror("connect error");
        exit(1);
    };


    write(1, "**connected to %s\n", sizeof("**connected to %s\n"));

    return tcpsock;
}

uint16_t handshake(int tcpsock, int udpport){
    //*****HANDSHAKE
    //**send hello
    char hello[3];
    hello[0] = 0;
    hello[1] = (uint8_t)(udpport >> 8);
    hello[2] = (uint8_t)udpport;

    int sendid = send(tcpsock, hello, sizeof(hello), 0);
    if(sendid<0){
        perror("error in sending hello");
        exit(1);
    }
    //TODO: check to see if sendid is less than sizeof(hello) and resend if so.
    write(1, "**hello message sent!\n", sizeof("**hello message sent!\n"));


    //**receive welcome message.
    char welcome[3];
    bzero(&welcome, sizeof(welcome));
    welcome[0] = 10;
    int recvid = recv(tcpsock, welcome, sizeof(welcome), 0);
    if(recvid<0){
        perror("error in receiving a welcome message!");
        exit(1);
    }
    else if(recvid == 0){
        printf("connection closed!\n");
        exit(1);
    }

    if((uint8_t)welcome[0] != 0){
        
        write(1, "handshake failed!\n", sizeof("handshake failed!\n"));
        exit(1);
    }

    
    write(1, "**welcome received!\n", sizeof("**welcome received!\n"));

    uint16_t numStations = (uint16_t) welcome[1] << 8;
    numStations += (uint8_t)welcome[2];
}

int setstation(int tcpsock, uint16_t station){
    char message1[3];
    message1[0] = 1;
    message1[1] = (uint8_t)(station >> 8);
    message1[2] = (uint8_t)station;

    int sentid = send(tcpsock, message1, sizeof(message1), 0);
    if(sentid<0){
        perror("failed to send SetStation message.\n");
        exit(1);
    }

    
    write(1, "**succeeded in sending SetStation message.\n", sizeof("**succeeded in sending SetStation message.\n"));


    //announcements!
    write(1, "**waiting for the announcement.\n", sizeof("**waiting for the announcement.\n"));

    char message[2];
    bzero(&message, sizeof(message));

    int recvid = recv(tcpsock, message, sizeof(message), 0);
    if(recvid<0){
        perror("error in receiving a welcome message!");
        exit(1);
    }
    else if(recvid == 0){
        write(1, "*****connection closed!\n", sizeof("*****connection closed!\n"));
        exit(1);
    }

    if((uint8_t)message[0] == 1){
        write(1, "**Anouncement: ", sizeof("**Anouncement: "));
    }

    else if((uint8_t)message[0] == 2){
        write(1, "**Invalid Command: ", sizeof("**Invalid Command: "));
    }



    uint8_t replySize = (uint8_t)message[1];
    char reply[replySize];

    recvid = recv(tcpsock, reply, sizeof(reply), 0);
    if(recvid<0){
        perror("error in receiving a welcome message!");
        exit(1);
    }
    else if(recvid == 0){
        write(1, "*****connection closed!\n", sizeof("*****connection closed!\n"));
        exit(1);
    }

    write(1, reply, replySize);
    write(1, "\n", sizeof("\n"));
    
}

int printMessage(int tcpsock){
    char message[2];
    bzero(&message, sizeof(message));

    int recvid = recv(tcpsock, message, sizeof(message), 0);
    if(recvid<0){
        perror("error in receiving a welcome message!");
        exit(1);
    }
    else if(recvid == 0){
        write(1, "*****connection closed!\n", sizeof("*****connection closed!\n"));
        exit(1);
    }

    if((uint8_t)message[0] == 1){
        write(1, "**Anouncement: ", sizeof("**Anouncement: "));
    }

    else if((uint8_t)message[0] == 2){
        write(1, "**Invalid Command: ", sizeof("**Invalid Command: "));
    }



    uint8_t replySize = (uint8_t)message[1];
    char reply[replySize];

    recvid = recv(tcpsock, reply, sizeof(reply), 0);
    if(recvid<0){
        perror("error in receiving a welcome message!");
        exit(1);
    }
    else if(recvid == 0){
        write(1, "*****connection closed!\n", sizeof("*****connection closed!\n"));
        exit(1);
    }

    write(1, reply, replySize);
    write(1, "\n", sizeof("\n"));
    
}

/*
 * 
 */
int main(int argc, char** argv) {

    char* servername;
    int serverport, udpport;

    //*****DEALING WITH ARGS
    if(argc < 4){
        write(1, "snowcast_control servername serverport udpport\n", sizeof("snowcast_control servername serverport udpport\n"));
        exit(1);
    }

    servername = argv[1];

    serverport = atoi(argv[2]);
    if(serverport < 1 || serverport > 0xffff){
        perror("serverport is invalid. i.e. > 0xffff or < 1 or not a number\n");
        exit(1);
    }

    udpport = atoi(argv[3]);
    if(udpport < 1 || udpport > 0xffff){
        perror("udpport is invalid. i.e. > 0xffff or < 1 or not a number\n");
        exit(1);
    }
    
    write(1, "starting the connection\n", sizeof("starting the connection\n"));

    //******CONNECTION
    int tcpsock = tcpsocket(servername, serverport);
    uint16_t num_stations = handshake(tcpsock, udpport);

    
    write(1, "**handshake complete!\n", sizeof("**handshake complete!\n"));



    char out[sizeof("**There are %d many stations. Please choose the station.\n") + 5];
    int size_out = sprintf(out, "**There are %d many stations. Please choose the station.\n", num_stations);
    
    write(1, out, size_out);

    fd_set master;
    fd_set read_fds;
    int maxfd = tcpsock;

    FD_ZERO(&master);
    FD_ZERO(&read_fds);

    FD_SET(tcpsock, &master);
    FD_SET(0, &master);

    for(;;){
        read_fds = master;

        if(select(maxfd+1, &read_fds, NULL, NULL, NULL) < 0){
            perror("select error!!");
            exit(1);
        }


        int i;
        for(i=0; i<=maxfd; i++){
            if(FD_ISSET(i, &read_fds)){
                //*****client's input
                if(i==0){
                    char in[5];
                    uint16_t station;
                    if(scanf("%s", in) == EOF){
                        write(1, "**failed to read the input.\n", sizeof("**failed to read the input.\n"));
                    }
                    
                    if(in[0] == 'q'){
                        close(tcpsock);
                        exit(1);
                    }

                    station = atoi(in);

                    setstation(tcpsock, station);
                }
                
                else if(i==tcpsock){
                    printMessage(tcpsock);
                }
            }
        }

    }



    return (EXIT_SUCCESS);
}

























