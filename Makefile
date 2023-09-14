# TODO fix include path
# TODO don't require libcbqn.so in build folder

CC=gcc
CFLAGS=-Wall -Wextra -Wpedantic -std=c11 -fPIC -I"../CBQN/include" -L. 
LIBS=-lcbqn -lsqlite3

all: libbqnsqlite.so

run: libbqnsqlite.so
	bqn ffitest.bqn

libbqnsqlite.so: bqnsqlite.o
	$(CC) -shared -L. $(LIBS) -olibbqnsqlite.so bqnsqlite.o

bqnsqlite.o: bqnsqlite.c
	$(CC) $(CFLAGS) -c bqnsqlite.c

.PHONY: clean
clean:
	rm bqnsqlite.o
	rm libbqnsqlite.so

.PHONY: test2
test2:
	gcc -lsqlite3 -o test2 test2.c
	./test2
