CFLAGS= -std=c11 -Wall -g -lm -fprofile-arcs -ftest-coverage -lgcov -fopenmp
CC=gcc
LIBSDIR=../library/

default: ex10
all: ex10 

.PHONY: clean all deps default run valgrind ex10 check 

ex10: deps
	$(CC) -o ex10 steiner_modified.c $(LIBSDIR)*.c $(CFLAGS) -I$(LIBSDIR)

deps: 
	$(MAKE) -C $(LIBSDIR) clean
	$(MAKE) -C $(LIBSDIR)

run: ex10
	./ex10 ../../data/b15.gph

valgrind: ex10
	valgrind ./ex10 ../../data/b15.gph

check:
	echo "add cppcheck"

clean:
	rm -f *.o ex10 *.gcno *.gcda
