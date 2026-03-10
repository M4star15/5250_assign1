CC = gcc
CFLAGS = -O3 -Wall -std=c99

OBJS = main.o matrix.o trimatrix.o Hessenberg.o QRdecomp.o QRA.o GetShift.o FindZero.o

main: $(OBJS)
	$(CC) $(OBJS) -o main -lm

main.o: main.c matrix.h trimatrix.h
	$(CC) $(CFLAGS) -c main.c

matrix.o: matrix.c matrix.h
	$(CC) $(CFLAGS) -c matrix.c

trimatrix.o: trimatrix.c trimatrix.h matrix.h
	$(CC) $(CFLAGS) -c trimatrix.c

Hessenberg.o: Hessenberg.c matrix.h trimatrix.h
	$(CC) $(CFLAGS) -c Hessenberg.c

QRdecomp.o: QRdecomp.c matrix.h
	$(CC) $(CFLAGS) -c QRdecomp.c

QRA.o: QRA.c trimatrix.h matrix.h
	$(CC) $(CFLAGS) -c QRA.c

GetShift.o: GetShift.c trimatrix.h
	$(CC) $(CFLAGS) -c GetShift.c

FindZero.o: FindZero.c trimatrix.h
	$(CC) $(CFLAGS) -c FindZero.c

clean:
	rm -f *.o main