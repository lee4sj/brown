.SUFFIXES:
.SUFFIXES: .c .o .net .lnx

CC = gcc
CFLAGS = -ggdb -Wall -Wextra -std=c99 -D_SVID_SOURCE -D_REENTRANT -D_XOPEN_SOURCE=500
LDFLAGS = -lpthread 

all: .depend A.lnx src.lnx node

UTIL=util/ipsum.c util/dbg.c util/htable.c
SRC=$(wildcard *.c)
HDR=$(wildcard *.h)
OBJ=$(SRC:.c=.o) $(UTIL:.c=.o)

node: $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJ) $(LIBS)

A.lnx: AB.net net2lnx 
	./net2lnx $<
src.lnx: loop.net net2lnx 
	./net2lnx $<

clean:
	rm -f node $(OBJ) ?.lnx .depend *~
	rm -f dst.lnx dstR.lnx long1.lnx long2.lnx short.lnx src.lnx srcR.lnx

.depend:  $(HDR) $(SRC) $(UTIL)
	$(CC) $(CFLAGS) -M $(SRC) $(UTIL) | perl -pe 's+^(\S*).o: util/\1+util/$$1.o: util/$$1+' > .depend

ifeq (.depend,$(wildcard .depend))
include .depend
endif
