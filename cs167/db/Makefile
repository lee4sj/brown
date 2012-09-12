CC=gcc

# Be sure to add the appropriate flag(s) for multithreaded programs.
CFLAGS=-g3 -Wall -Wextra -Werror -pedantic \
	-Wconversion -Wcast-qual -Wcast-align -Winline -Wfloat-equal \
	-Wnested-externs -Wno-unused-function -Wno-unused-parameter -pthread\
	-std=c99 -DAUTOMATED_TEST

all: server interface

server: server.o db.o window.o
	$(CC) $(CFLAGS) server.o db.o window.o -o server

server.o: server.c window.h db.h
	$(CC) server.c -c $(CFLAGS)

db.o: db.c db.h
	$(CC) db.c -c $(CFLAGS)

window.o: window.c window.h
	$(CC) window.c -c $(CFLAGS)

interface: interface.c
	$(CC) -o interface interface.c $(CFLAGS) -O

clean:
	/bin/rm -f *.o server interface
