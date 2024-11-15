CC = gcc

CFLAGS = -g -c

all: c-sar

clean: c-sar
	rm *.o

c-sar: c-sar.o
	$(CC) $(LDFLAGS) c-sar.o -o c-sar

c-sar.o: src/c-sar.c
	$(CC) $(CFLAGS) src/c-sar.c -o c-sar.o
