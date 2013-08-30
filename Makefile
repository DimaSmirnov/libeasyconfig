all: example

example: example.c libeasyconfig.c
	gcc -g -o example example.c libeasyconfig.c -std=c99
