CFLAGS= -std=c11 -Wall -g -lm -fprofile-arcs -ftest-coverage -lgcov
CC=gcc
LIBSDIR=../library/

default: ex8
all: ex8 

.PHONY: clean all deps default run valgrind ex8 check

ex8: deps
	$(CC) -o ex8 steiner.c $(LIBSDIR)*.c $(CFLAGS) -I$(LIBSDIR)

deps: 
	$(MAKE) -C $(LIBSDIR) clean
	$(MAKE) -C $(LIBSDIR)

run: ex8
	./ex8 ../../data/b15.gph

valgrind: ex8
	valgrind ./ex8 ../../data/b15.gph

check:
	cppcheck . 2> result.xml
	cat result.xml

coverage: run
	gcov *.c $(LIBSDIR)*.c
	lcov --capture --directory . --output-file coverage.info
	genhtml coverage.info --output-directory coverage

clean:
	rm -f *.o ex8 *.gcno *.gcda coverage*
