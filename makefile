CC = gcc
CFLAGS = -I.
DEPS = hwutils.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

hw: hwutils.o main.o
	gcc -o hw hwutils.o main.o -I. -lm -lpthread

clean:
	rm -rf *.o hw
