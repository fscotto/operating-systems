CC=gcc
CFLAGS=-Wall -o mem
SRCS=main.c

build:
	$(CC) $(CFLAGS) $(SRCS)
debug:
	$(CC) $(CFLAGS) -fsanitize=address -static-libasan -ggdb $(SRCS)
	gdb -q mem
clean:
	rm -f mem
