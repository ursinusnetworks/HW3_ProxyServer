CC=gcc
CFLAGS=-g -Wall -pedantic

all: server tester test httpheadertest

md5.o: md5-c/md5.c md5-c/md5.h
	$(CC) $(CFLAGS) -c md5-c/md5.c

hashmap.o: hashmap.c hashmap.h
	$(CC) $(CFLAGS) -c hashmap.c

arraylist.o: arraylist.c arraylist.h
	$(CC) $(CFLAGS) -c arraylist.c

http.o: http.c http.h hashmap.o arraylist.o
	$(CC) $(CFLAGS) -c http.c

util.o: util.c util.h hashmap.o arraylist.o md5.o
	$(CC) $(CFLAGS) -c util.c 

server: server.c hashmap.o arraylist.o http.o util.o md5.o
	$(CC) $(CFLAGS) -o server server.c hashmap.o arraylist.o http.o util.o md5.o -lpthread

tester: arraylist.o tester.c
	$(CC) $(CFLAGS) -o tester tester.c arraylist.o

httpheadertest: httpheadertest.c hashmap.o arraylist.o http.o
	$(CC) $(CFLAGS) -o httpheadertest httpheadertest.c hashmap.o arraylist.o http.o

test: test.c util.o arraylist.o hashmap.o md5.o
	$(CC) $(CFLAGS) -o test test.c util.o arraylist.o md5.o

clean:
	rm server tester test httpheadertest *.o