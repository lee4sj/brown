CC = gcc
DEBUGFLAGS = -g -Wall
CFLAGS = -D_REENTRANT $(DEBUGFLAGS) -D_XOPEN_SOURCE=500
LDFLAGS = -lpthread 

all: snowcast_listener snowcast_control snowcast_server

snowcast_listener: snowcast_listener.c
snowcast_control:  snowcast_control.c extrafile1.c extrafile2.c
snowcast_server:   snowcast_sever.c extrafile3.c extrafile4.c
clean:
	rm -f snowcast_listener snowcast_control snowcast_server
