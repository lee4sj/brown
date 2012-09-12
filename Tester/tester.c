/* 
 * File:   tester.c
 * Author: sl136
 *
 * Created on February 3, 2011, 8:34 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <netdb.h>

typedef struct request{
    uint8_t replyType;
    uint16_t udpPort;
    } request;
/*
 * 
 */
int main(int argc, char** argv) {

   /* char hello[123];
    hello[0] = 0;
    hello[1] = (uint8_t)(0x00ff >> 8);
    hello[2] = (uint8_t)0xffff;

    //printf("%d\n%d\n%d\n", sizeof(hello), hello[1], hello[2]);


    struct hostent *host = gethostbyname("google.com");
    printf("%x    %x", (uint32_t)host->h_addr_list[0]);

    char *yo = "yoyoyoyoyoyo";
    int a = strlen(yo);

    printf("%d", a);

    int *a2 = (int*)malloc(5*sizeof(int));
    free(NULL);*/


    /*FILE* m_file;
    long file_size;

    m_file = fopen("/course/cs168/pub/snowcast/mp3/U2-StuckInAMoment.mp3",  "r");
    if(m_file == NULL){
        perror("error in opening the mp3 file!!");
        exit(1);
    }

    fseek(m_file, 0, SEEK_END);
    file_size = ftell(m_file);
    rewind(m_file);


    int packet_size = 8;
    char buffer[packet_size];

    size_t result = fread(buffer, 1, (long)packet_size, m_file);

    fseek(m_file, result, SEEK_CUR);
    write(1, buffer, result);
    printf("***** %d: \n", result);

    result = fread(buffer, 1, (long)packet_size, m_file);
    write(1, buffer, result);
    printf("***** %d: \n", result);*/

    printf("%d", 999999999/16);




    return (EXIT_SUCCESS);
}

