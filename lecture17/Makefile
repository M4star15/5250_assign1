CC = gcc
CFLAGS = -Wall -O3 -std=c99 -fopenmp
LDFLAGS = -lgomp

all: main.exe

main.exe: main.o
	$(CC) main.o -o main.exe $(LDFLAGS)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

clean:
	rm -f *.o main.exe