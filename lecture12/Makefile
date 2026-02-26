CC = gcc
CFLAGS = -Wall -lm

main: main.o
	$(CC) main.o -o main -lm

main.o: main.c stack.h
	$(CC) $(CFLAGS) -c main.c

clean:
	rm -f *.o main