CC = gcc
DEBUGFLAGS = -g -Wall
CFLAGS = -D_REENTRANT $(DEBUGFLAGS) $(LDFLAGS) -D_XOPEN_SOURCE=500 -std=c99
LDFLAGS = -lpthread 
OBJECTS = node.o link_layer.o network_layer.o rip.o parselinks.o dbg.o ipsum.o htable.o

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

parselinks.o: parselinks.c parselinks.h
	$(CC) $(CFLAGS) -c parselinks.c

dbg.o: util/dbg.c util/dbg.h util/dbg_modes.h
	$(CC) $(CFLAGS) -c util/dbg.c

ipsum.o: util/ipsum.c util/ipsum.h
	$(CC) $(CFLAGS) -c util/ipsum.c

htable.o: util/htable.c util/htable.h
	$(CC) $(CFLAGS) -c util/htable.c

clean:
	rm -f node $(OBJECTS)
