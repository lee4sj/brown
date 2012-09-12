CC = gcc
DEBUGFLAGS = -g -Wall
LDFLAGS = 
OBJECTS = server_tuntap.o base64.o dns_builder.o

all: server_tuntap

server_tuntap: $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o server_tuntap

server_tuntap.o: server_tuntap.c
	$(CC) -c server_tuntap.c 

base64.o: base64.c base64.h
	$(CC) -c base64.c 

dns_builder.o: dns_builder.c dns_builder.h
	$(CC) -c dns_builder.c 

clean:
	rm -f server_tuntap $(OBJECTS)
