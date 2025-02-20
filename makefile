CC=gcc
CFLAGS=-g -Wall -pedantic

all: server tester test httpheadertest

hashmap.o: hashmap.c hashmap.h
	$(CC) $(CFLAGS) -c hashmap.c

arraylist.o: arraylist.c arraylist.h
	$(CC) $(CFLAGS) -c arraylist.c

http.o: http.c http.h hashmap.o arraylist.o
	$(CC) $(CFLAGS) -c http.c

util.o: util.c util.h hashmap.o arraylist.o
	$(CC) $(CFLAGS) -c util.c -lssl -lcrypto

server: server.c hashmap.o arraylist.o http.o util.o
	$(CC) $(CFLAGS) -o server server.c hashmap.o arraylist.o http.o util.o -lssl -lcrypto 

tester: arraylist.o tester.c
	$(CC) $(CFLAGS) -o tester tester.c arraylist.o

httpheadertest: httpheadertest.c hashmap.o arraylist.o http.o
	$(CC) $(CFLAGS) -o httpheadertest httpheadertest.c hashmap.o arraylist.o http.o

test: test.c util.o arraylist.o hashmap.o
	$(CC) $(CFLAGS) -o test test.c util.o arraylist.o -lssl -lcrypto 

clean:
	rm server tester test httpheadertest *.o