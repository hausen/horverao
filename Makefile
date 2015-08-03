CC=gcc
CFLAGS=-g -Wall -Wextra -pedantic

all: teste

teste: teste.c horverao.o
	gcc $(CFLAGS) -g $^ -o $@

%.o: %.c %.h
	gcc -c $(CFLAGS) $< -o $@

clean:
	rm -f horverao.o teste
