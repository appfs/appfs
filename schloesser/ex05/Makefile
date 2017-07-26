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
	cppcheck . 2> result.xml
	cat result.xml

coverage: run
	gcov *.c $(LIBSDIR)*.c -o . 
	lcov --capture --directory . --output-file coverage.info
	genhtml coverage.info --output-directory coverage

clean:
	rm -rf *.o ex5 *.gcno *.gcda coverage*
