CC = gcc
CFLAGS = -I.
DEPS = hwutils.h

%.o: %.c $(DEPS)
	$(CC) -g -c -o $@ $< $(CFLAGS)

hw: hwutils.o main.o
	gcc -o hw hwutils.o main.o -I. -lm -lpthread

test: hwutils.o test.o
	gcc -g -o test hwutils.o test.o -I. -lm -lpthread

clean:
	rm -rf *.o hw test
