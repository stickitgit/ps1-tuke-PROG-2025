CC = gcc
CFLAGS = -std=c11 -Wall -Werror -lm

all: ps1

playfair.o: playfair.c playfair.h
	$(CC) $(CFLAGS) -c playfair.c

bmp.o: bmp.c bmp.h
	$(CC) $(CFLAGS) -c bmp.c

ps1: main.c playfair.o bmp.o
	$(CC) $(CFLAGS) -o ps1 main.c playfair.o bmp.o

clean:
	rm -f *.o ps1