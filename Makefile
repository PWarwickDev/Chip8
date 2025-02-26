CC=gcc
CFLAGS=-I

chip8: main.o
	$(CC) main.o -o chip8

main.o: main.c
	$(CC) -c main.c

clean:
	rm main.o chip8

