all: example

example: example.c libeasyconfig.c
	g++ -g -o example example.c libeasyconfig.c
