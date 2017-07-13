CFLAGS= -std=c11 -Wall -g -lm -fprofile-arcs -ftest-coverage -lgcov
CC=gcc
LIBSDIR=../library/

default: main
all: main 

.PHONY: clean all deps default run valgrind main check coverage

main: deps
	$(CC) -o main longest_shortest.c $(LIBSDIR)*.c $(CFLAGS) -I$(LIBSDIR)

deps: 
	$(MAKE) -C $(LIBSDIR) clean
	$(MAKE) -C $(LIBSDIR)

run: main
	./main ../data/small.gph

valgrind: main
	valgrind ./main ../data/small.gph

coverage: run
	gcov longest_shortest.c graph.c misc.c heap.c

clean:
	rm -f *.o main *.gcno *.gcda
