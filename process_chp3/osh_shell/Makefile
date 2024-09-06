CC=gcc
CFLAGS=-o osh -Wall
SRCS=history.c osh.c

.DEFAULT_GOAL := build

build:
	$(CC) $(CFLAGS) -fsanitize=address -static-libasan $(SRCS)
	
release:
	$(CC) $(CFLAGS) $(SRCS)

debug:
	$(CC) $(CFLAGS) -fsanitize=address -static-libasan -ggdb $(SRCS)
	gdb -q osh

clean:
	rm -f osh
