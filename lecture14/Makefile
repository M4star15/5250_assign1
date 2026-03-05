CC=gcc
CFLAGS=-O3 -Wall -std=c99

main: main.o matrix.o
	$(CC) main.o matrix.o -o main -lm

main.o: main.c matrix.h
	$(CC) $(CFLAGS) -c main.c

matrix.o: matrix.c matrix.h
	$(CC) $(CFLAGS) -c matrix.c

clean:
	rm -f *.o main