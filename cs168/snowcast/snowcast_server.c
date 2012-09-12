/* 
 * File:   snowcast_server.c
 * Author: sl136
 *
 * Created on February 3, 2011, 7:01 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdint.h>
#include <pthread.h>
#include <sys/time.h>


int tcpsocket(int port, int addr){
    int fd = socket (AF_INET, SOCK_STREAM, 0);
    if(fd < 0){
        perror("socket");
        exit(1);
    }

    struct sockaddr_in sin;
    bzero (&sin, sizeof (sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons (port);
    sin.sin_addr.s_addr = htonl (addr);

    if(bind(fd, (struct sockadder *) &sin, sizeof(sin)) < 0){
        perror("bind");
        exit(1);
    };

    return fd;
}

/**
 *  the root of client will always be set to simple dummy. This is to resolve around deletion of root.
 **/
struct client {
    int cfd;
    struct sockaddr_in sockaddr_cli;
    uint16_t udpport;
    uint16_t station;
    struct client *c;
};

struct threadArgs{
    int station_number;
    int num_stations;
    fd_set *master;
    char* file;
    struct client *root;
    pthread_mutex_t *mutex;
};

struct client* getCli(struct client* root, int fd){
    /*if(mutex != NULL){
        int i;
        for(i = 0; i < num_stations; i++)
            pthread_mutex_lock(&mutex[i]);
    }*/

    int popped = 0;
    struct client *current = root;

    while(!popped){

        if(current == NULL || current->cfd == fd){
            popped = 1;
        }
        else{
            current = current->c;
        }
    }

    /*if(mutex != NULL){
        int i;
        for(i = 0; i < num_stations; i++)
            pthread_mutex_unlock(&mutex[i]);
    }*/

    return current;
}

struct client* popCli(struct client* root, int fd, pthread_mutex_t *mutex, int num_stations){

    if(mutex != NULL){
        int i;
        for(i = 0; i < num_stations; i++)
            pthread_mutex_lock(&mutex[i]);
    }


    int popped = 0;
    struct client *current = root;
    struct client *previous = root;

    while(!popped){

        if(current == NULL){
            popped = 1;
        }
        else if(current->cfd == fd){
            previous->c = current->c;
            current->c = NULL;
            popped = 1;
        }
        else{
            previous = current;
            current = current->c;
        }
    }

    if(mutex != NULL){
        int i;
        for(i = 0; i < num_stations; i++)
            pthread_mutex_unlock(&mutex[i]);
    }

    return current;

}


int addToStation(uint16_t station_number, char* filename, struct client *cli, fd_set *master, struct client *root, pthread_mutex_t *mutex, int num_stations){
    //set the station number

    if(cli == NULL){
        write(1, "**The addToStation called with NULL client!\n", sizeof("**The addToStation called with NULL client!\n"));

        return 0;
    }else{
        cli->station = station_number;
        
        //announce to the client
        int cfd = cli->cfd;
        char message[2];
        message[0] = 1;
        message[1] = strlen(filename);

        int sendid = send(cfd, message, sizeof(message), 0);
        if(sendid<0){
            printToSTD(cfd, "");
            perror("error in sending message");


            //closing the connection.
            FD_CLR(cfd, master);
            free(popCli(root, cfd, mutex, num_stations));
            close(cfd);
            printToSTD(cfd, "closed the connection.\n");
        }

        sendid = send(cfd, filename, strlen(filename), 0);
        if(sendid<0){
            printToSTD(cfd, "");
            perror("error in sending message");

            //closing connection
            FD_CLR(cfd, master);
            free(popCli(root, cfd, mutex, num_stations));
            close(cfd);
            printToSTD(cfd, "closed the connection.");
        }


        printToSTD(cfd, "sent announcement.\n");

        
        return 1;
    }
}

int addToCliList(struct client *root, struct client *cli, pthread_mutex_t *mutex, int num_stations){
    if(mutex != NULL){
        int i;
        for(i = 0; i < num_stations; i++)
            pthread_mutex_lock(&mutex[i]);
    }
    

    cli->c = root->c;
    root->c = cli;

    if(mutex != NULL){
        int i;
        for(i = 0; i < num_stations; i++)
            pthread_mutex_unlock(&mutex[i]);
    }

    return 1;

}

int sendInvalidMessage(int fd, char* reply, fd_set *master, struct client *root, pthread_mutex_t *mutex, int num_stations){

    char message[2];
    message[0] = 2;
    message[1] = strlen(reply);

    int sendid = send(fd, message, sizeof(message), 0);
    if(sendid<0){
        printToSTD(fd, "");
        perror("error in sending message");

        //closing connection
        FD_CLR(fd, master);
        free(popCli(root, fd, mutex, num_stations));
        close(fd);
        printToSTD(fd, "closed the connection.\n");
    }

    sendid = send(fd, reply, strlen(reply), 0);
    if(sendid<0){
        printToSTD(fd, "");
        perror("error in sending message");

        //closing connection
        FD_CLR(fd, master);
        free(popCli(root, fd, mutex, num_stations));
        close(fd);
        printToSTD(fd, "closed the connection.\n");
        
    }


    printToSTD(fd, "**invalid command message received.\n");
}

int printStations(uint16_t num_stations, struct client *root){

    uint16_t i;
    for(i = 0; i<num_stations; i++){

        char out[sizeof("**station : ") + 5];
        int size = sprintf(out, "station %d: ", i);
        write(1, out, size);

        struct client *current = root;

        while(current != NULL){
            if(current->station == i){
                size = sprintf(out, "%d, ", current->cfd);
                write(1, out, size);
            }
            current = current->c;
        }
        write(1, "\n", sizeof("\n"));
    }
}

int printToSTD(int fd, char* line){

    char out[sizeof("**client : ") + 5];
    int size = sprintf(out, "client %d: ", fd);
    write(1, out, size);
    write(1, line, strlen(line));
}

void *station_thread(struct threadArgs *input){
    struct client *root = input->root;
    struct client *current = input->root->c;
    int station_number = input->station_number;
    int num_stations = input->num_stations;
    pthread_mutex_t *mutex = input->mutex;
    fd_set *master = input->master;

    FILE* m_file;
    long file_size;

    m_file = fopen(input->file, "r");
    if(m_file == NULL){
        perror("error in opening the mp3 file!!");
        exit(1);
    }

    fseek(m_file, 0, SEEK_END);
    file_size = ftell(m_file);
    rewind(m_file);


    int packet_size = 512;
    char buffer[packet_size];

    int index = 0;
    

    int udpfd = socket(AF_INET, SOCK_DGRAM, 0);

    struct timeval tv_begin;
    struct timeval tv_end;
    struct timespec tv;

    for(;;){

        gettimeofday(&tv_begin, NULL);

        int sendingSize = packet_size;
        size_t result = fread(buffer, 1, (long)packet_size, m_file);
        if(result <= packet_size){
            sendingSize = result;
        }

        if(ftell(m_file) <= file_size){
            fseek(m_file, sendingSize, SEEK_CUR);
        }else{
            fseek(m_file, 0, SEEK_SET);
        }


        pthread_mutex_lock(mutex);

        //Stream songs to everyone in the list.
        while(current != NULL){
            if(current->station == station_number){
                int sent = sendto(udpfd, &buffer, sendingSize * sizeof(char), 0, &(current->sockaddr_cli), sizeof(current->sockaddr_cli));
                if(sent < 0){
                    printToSTD(current->cfd, "");
                    perror("error in sending message.");

                    char out[10];
                    int a = sprintf(out, "%d\n", ntohs(current->sockaddr_cli.sin_port));
                    write(1, out, a);

                    FD_CLR(current->cfd, master);
                    close(current->cfd);
                    printToSTD(current->cfd, "closed the connection.\n");

                    pthread_mutex_unlock(mutex);
                    free(popCli(root, current->cfd, mutex, num_stations));
                    pthread_mutex_lock(mutex);
                }
            }

            
            current = current->c;
        }

        index += packet_size;
        current = root->c;

        pthread_mutex_unlock(mutex);

        gettimeofday(&tv_end, NULL);

        tv.tv_sec = (tv_end.tv_sec) - (tv_begin.tv_sec);
        tv.tv_nsec = (long)(tv_end.tv_usec - tv_begin.tv_usec);

        if(tv.tv_sec < 1){
            tv.tv_sec = 0;
            tv.tv_nsec = 31100000 - tv.tv_nsec;
            nanosleep(&tv, NULL);
        }
    }
}

/*
 * 
 */
int main(int argc, char** argv) {

    //*****DEALING WITH ARGUEMENTS!!
    if(argc < 3){
        printf("wrong number of arguments! (%i)\n", (argc-1));
        exit(1);
    }

    int port = atoi(argv[1]);

    if(port < 1 || port > 0xffff){
        perror("port is invalid. i.e. > 0xffff or < 1 or not a number");
        exit(1);
    }

    //do something with the musics
    int num_stations = argc - 2;
    char* stations[num_stations];
    pthread_mutex_t thrmutex[num_stations];

    fd_set master;
    fd_set readfds;
    int maxfd;

    FD_ZERO(&master);
    FD_ZERO(&readfds);


    struct sockaddr_in temp;

    struct client *root = (struct client *) malloc(sizeof(struct client));
    root->cfd = -1;
    memcpy(&root->sockaddr_cli, &temp, sizeof(temp));
    root->station = -1;
    root->udpport = -1;
    root->c = NULL;
    
    struct threadArgs thread_args[num_stations];

    int i;
    for(i = 0; i<num_stations; i++){
        stations[i] = argv[i+2];
        pthread_mutex_init(&thrmutex[i], NULL);
        thread_args[i].station_number = i;
        thread_args[i].num_stations = num_stations;
        thread_args[i].master = &master;
        thread_args[i].file = stations[i];
        thread_args[i].root = root;
        thread_args[i].mutex = &thrmutex[i];
    }



    //*****INITIATION OF THREADS FOR STATIONS!
    
    
    pthread_t threads[num_stations];

    for(i=0; i<num_stations; i++){
        int t = pthread_create(&threads[i], NULL, station_thread, &thread_args[i]);
        if(t != 0){
            perror("failed to create threads for the stations!");
            exit(1);
        }
    }



    //*****SOCKET CONNECTION!
    //socket part
    int fd = tcpsocket(port, INADDR_ANY);
    FD_SET(0, &master);
    FD_SET(fd, &master);

    if(listen (fd, 5) < 0){
        close(fd);
        perror("could not listen!");
        exit(1);
    }

    struct sockaddr_in sin;
    bzero (&sin, sizeof (sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons (port);
    sin.sin_addr.s_addr = htonl (INADDR_ANY);
    socklen_t sinlen;

    struct client helloList = {-1, temp, -1, -1, NULL};
    fd_set hello;

    FD_ZERO(&hello);

    maxfd = fd;

    for(;;){

        readfds = master;

        if(select(maxfd+1, &readfds, NULL, NULL, NULL) < 0){
            perror("select error");
            exit(1);
        }


        int i;
        for(i = 0; i <= maxfd; i++){
            if(FD_ISSET(i, &readfds)){

                //if the server requests from stdin
                if(i == 0){
                    char in[1];
                    if(scanf("%s", in) == EOF){
                        write(1, "**failed to read the input.\n", sizeof("**failed to read the input.\n"));
                    }

                    //quit the server
                    if(in[0] == 'q'){
                        close(fd);
                        exit(1);
                    }

                    //print out the station list;
                    if(in[0] == 'p'){
                        printStations(num_stations, root);
                    }
                }

                //if new client connects to the server.
                else if(i == fd){
                    
                    struct client *newcli = (struct client*) malloc(sizeof(struct client));
                    bzero(newcli, sizeof(struct client));
                    sinlen = sizeof(newcli->sockaddr_cli);

                    int newfd = accept(fd, (struct sockaddr *) &newcli->sockaddr_cli, &sinlen);
                    if(newfd < 0){
                        free(newcli);
                        perror("accept error");
                    }else{
                        FD_SET(newfd, &master);
                        if(newfd > maxfd)
                            maxfd = newfd;

                        //add to hello list
                        FD_SET(newfd, &hello);

                        
                        newcli->cfd = newfd;
                        newcli->udpport = -1;
                        newcli->station = -1;
                        newcli->c = NULL;
                        
                        addToCliList(&helloList, newcli, NULL, 0);

                        printToSTD(i, "connection accepted. Expecting hello.\n");
                    }
                }

                //if the client has not sent a hello yet
                else if(FD_ISSET(i, &hello)){

                    FD_CLR(i, &hello);

                    char hello[3];
                    int receive_hello = recv(i, hello, sizeof(hello), 0);
                    if(receive_hello < 0){

                        printToSTD(i, "");
                        perror("Failed to receive the request!");

                        //closing connection
                        FD_CLR(i, &master);
                        free(popCli(root, i, thrmutex, num_stations));
                        close(i);
                        printToSTD(i, "closed the connection.\n");
                    }else if(receive_hello == 0){
                        //closing connection
                        FD_CLR(i, &master);
                        free(popCli(root, i, thrmutex, num_stations));
                        close(i);

                        printToSTD(i, "the client closed the connection.\n");
                    }else{

                        if((uint8_t)hello[0] != 0){

                            sendInvalidMessage(i, "wrong type of request. hello expected.", &master, root, thrmutex, num_stations);
                            printToSTD(i, "wrong type of request requested.\n");


                            //closing connection
                            FD_CLR(i, &master);
                            free(popCli(root, i, thrmutex, num_stations));
                            close(i);
                            printToSTD(i, "closed the connection.\n");
                        }else{
                            uint16_t cport = (uint16_t)hello[1] << 8;
                            cport += (uint8_t)hello[2];

                            //update the client's port
                            struct client *cli = popCli(&helloList, i, NULL, 0);
                            
                            if(cli == NULL){
                                printToSTD(i, "error with connection, the client could not be found.\n");

                                FD_CLR(i, &master);
                                close(i);
                            }else{
                                cli->udpport = cport;
                                cli->sockaddr_cli.sin_port = htons(cport);
                                cli->sockaddr_cli.sin_family = AF_INET;


                                addToCliList(root, cli, &thrmutex, num_stations);

                                //send welcome
                                char welcome[3];
                                welcome[0] = 0;
                                welcome[1] = (uint8_t)(num_stations >> 8);
                                welcome[2] = (uint8_t)num_stations;

                                send(i, welcome, sizeof(welcome), 0);

                                printToSTD(i, "hello received. welcome sent. expecting station.\n");
                            }
                        }
                    }
                }

                //if the client has completed the handshake
                else{
                    
                    char message[3];
                    int received = recv(i, message, sizeof(message), 0);
                    if(received < 0){
                        printToSTD(i, "");
                        perror("Failed to receive the request!");

                        //closing connection
                        FD_CLR(i, &master);
                        free(popCli(root, i, thrmutex, num_stations));
                        close(i);
                        
                        printToSTD(i, "closed the connection.\n");
                    }else if(received == 0){
                        //closing connection
                        FD_CLR(i, &master);
                        free(popCli(root, i, thrmutex, num_stations));
                        close(i);
                        
                        printToSTD(i, "the client closed the connection.\n");
                    }else{
                        if((uint8_t)message[0] != 1){

                            sendInvalidMessage(i, "wrong type of request. setstation request expected.", &master, root, thrmutex, num_stations);
                            printToSTD(i, "invalid commmand received. corresponding message sent.\n");

                            //closing connection
                            FD_CLR(i, &master);
                            free(popCli(root, i, thrmutex, num_stations));
                            close(i);
                            printToSTD(i, "closed the connection.\n");
                        }else{

                            uint16_t station = (uint16_t) message[1] << 8;
                            station += (uint8_t)message[2];
                            station = station;

                            //check to see if the station exists.
                            if(station >= num_stations){
                                char *out;
                                int size = sprintf(out, "the station %d does not exist.", station);
                                sendInvalidMessage(i, out, &master, root, thrmutex, num_stations);

                                //closing connection
                                FD_CLR(i, &master);
                                free(popCli(root, i, thrmutex, num_stations));
                                close(i);
                                printToSTD(i, "closed the connection.\n");
                            }else{
                                if(addToStation(station, stations[station], getCli(root, i),
                                                &master, root, thrmutex, num_stations) == 0){
                                    printToSTD(i, "the client's information could not be found in the root list.\n");

                                    FD_CLR(i, &master);
                                    close(i);
                                    printToSTD(i, "closed the connection.\n");
                                }
                            }
                        }
                    }
                }
            }
        }      
    }


}

























