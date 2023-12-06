CC=gcc
CFLAGS=-Wall -Wextra -g -pedantic

all: test.out
	./$^

test.out: test.c
	$(CC) $(CFLAGS) -o $@ $^
