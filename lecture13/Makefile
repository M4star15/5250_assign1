CC = gcc
CFLAGS = -O3 -Wall -std=c99
LDFLAGS = -lm

all: main

main: main.o matrix.o
	$(CC) main.o matrix.o -o main $(LDFLAGS)

main.o: main.c matrix.h
	$(CC) $(CFLAGS) -c main.c

matrix.o: matrix.c matrix.h
	$(CC) $(CFLAGS) -c matrix.c

clean:
	rm -f *.o main