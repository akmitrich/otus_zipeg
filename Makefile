CC=gcc
CFLAGS=-Wall -Wextra -Wpedantic -std=c11

zipeg: zipeg.c slice.o
	$(CC) $(CFLAGS) -o zipeg zipeg.c slice.o

slice.o: slice.h slice.c
	$(CC) $(CFLAGS) -c slice.c