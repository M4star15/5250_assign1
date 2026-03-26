CC = gcc
CFLAGS = -O3 -Wall -std=c99 -fopenmp
LDFLAGS = -lm

all: main.exe

main.exe: main.o
	$(CC) $(CFLAGS) main.o -o main.exe $(LDFLAGS)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

clean:
	rm -f *.o main.exe