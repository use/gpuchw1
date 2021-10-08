CC = gcc
CFLAGS = -I.
DEPS = hwutils.h

%.o: %.c $(DEPS)
	$(CC) -g -c -o $@ $< $(CFLAGS)

hw1: hwutils.o main.o timing.o
	gcc -o hw1 hwutils.o main.o timing.o -I. -lm -lpthread
	./hw1

test: hwutils.o test.o
	gcc -g -o test hwutils.o test.o -I. -lm -lpthread

clean:
	rm -rf *.o hw1 test

all: clean test hw1
