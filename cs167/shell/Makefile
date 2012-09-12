EXEC =		sh
OBJS = 		sh.o
CFLAGS =    -g3 -Wall -Wextra -Wconversion -Wcast-qual -Wcast-align
CFLAGS +=   -Winline -Wfloat-equal -Wnested-externs
CFLAGS +=   -pedantic -std=c99 -Werror
CC =		/usr/bin/gcc

.PHONY: all
all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) -o $@ $(CFLAGS) $(OBJS) 

.PHONY: clean
clean:
	rm -f *.o $(OBJS) $(EXEC)

