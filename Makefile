CC=gcc
CFLAGS=-lSDL2

chip8: main.o
	$(CC) main.o -o chip8 $(CFLAGS)

main.o: main.c
	$(CC) -c main.c

clean:
	rm main.o chip8

