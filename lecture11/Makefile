CC = gcc
CFLAGS = -Wall -lm

OBJ = main.o poly.o poly_add.o

all: main

main: $(OBJ)
	$(CC) $(OBJ) -o main -lm

main.o: main.c poly.h
poly.o: poly.c poly.h
poly_add.o: poly_add.c poly.h

clean:
	rm -f *.o main