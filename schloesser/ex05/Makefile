CFLAGS= -std=c11 -Wall -g -lm -fprofile-arcs -ftest-coverage -lgcov
CC=gcc
LIBSDIR=../library/

default: ex5
all: ex5

.PHONY: clean all deps default run valgrind ex5 check coverage

ex5: deps
	$(CC) -o ex5 longest_shortest.c $(LIBSDIR)*.c $(CFLAGS) -I$(LIBSDIR)

deps: 
	$(MAKE) -C $(LIBSDIR) clean
	$(MAKE) -C $(LIBSDIR)

run: ex5
	./ex5 ../../data/b15.gph

valgrind: ex5
	valgrind ./ex5 ../../data/b15.gph

check:
	echo "add cppcheck"

coverage: run
	gcov longest_shortest.c graph.c misc.c heap.c

clean:
	rm -f *.o ex5 *.gcno *.gcda
