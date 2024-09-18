CC=gcc
CFLAGS=-o cancel_threads -Wall -lpthread -lc -D_GNU_SOURCE
SRCS=main.c

.DEFAULT_GOAL := build

build:
	$(CC) $(CFLAGS) -fsanitize=address -static-libasan $(SRCS)
	
release:
	$(CC) $(CFLAGS) $(SRCS)

debug:
	$(CC) $(CFLAGS) -fsanitize=address -static-libasan -ggdb $(SRCS)
	gdb -q cancel_threads

clean:
	rm -f cancel_threads
