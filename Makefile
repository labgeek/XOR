all:  xor

xor:    Makefile xor.c
	gcc -g -Wall -o xor xor.c

clean:
	rm xor
