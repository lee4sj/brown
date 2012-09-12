CC = gcc
DEBUGFLAGS = -g -Wall
CFLAGS = -D_REENTRANT $(DEBUGFLAGS) $(LDFLAGS) -D_GNU_SOURCE -D_XOPEN_SOURCE=500 -std=c99 -lrt
LDFLAGS = -lpthread 
OBJECTS = node.o link_layer.o network_layer.o rip.o state_machine.o socket_api.o transport_layer.o parselinks.o dbg.o ipsum.o htable.o bqueue.o tcp_thread.o

all: node

node: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o node

node.o: node.c
	$(CC) $(CFLAGS) -c node.c

link_layer.o: link_layer.c link_layer.h
	$(CC) $(CFLAGS) -c link_layer.c

network_layer.o: network_layer.c network_layer.h
	$(CC) $(CFLAGS) -c network_layer.c

rip.o: rip.c rip.h
	$(CC) $(CFLAGS) -c rip.c

state_machine.o: state_machine.c state_machine.h
	$(CC) $(CFLAGS) -c state_machine.c

socket_api.o: socket_api.c socket_api.h
	$(CC) $(CFLAGS) -c socket_api.c

transport_layer.o: transport_layer.c transport_layer.h
	$(CC) $(CFLAGS) -c transport_layer.c

parselinks.o: parselinks.c parselinks.h
	$(CC) $(CFLAGS) -c parselinks.c

dbg.o: util/dbg.c util/dbg.h util/dbg_modes.h
	$(CC) $(CFLAGS) -c util/dbg.c

ipsum.o: util/ipsum.c util/ipsum.h
	$(CC) $(CFLAGS) -c util/ipsum.c

htable.o: util/htable.c util/htable.h
	$(CC) $(CFLAGS) -c util/htable.c

bqueue.o: util/bqueue.c util/bqueue.h
	$(CC) $(CFLAGS) -c util/bqueue.c

tcp_thread.o: tcp_thread.c tcp_thread.h
	$(CC) $(CFLAGS) -c tcp_thread.c

clean:
	rm -f node $(OBJECTS)
