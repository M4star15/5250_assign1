CC = gcc
CFLAGS = -O3 -Wall -std=c99 -fopenmp
LDFLAGS = -lm -fopenmp

all: main.exe

main.exe: main.c
	$(CC) $(CFLAGS) main.c -o main.exe $(LDFLAGS)

clean:
	rm -f main.exe *.png *.data *.log *.txt