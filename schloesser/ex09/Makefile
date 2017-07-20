CFLAGS= -std=c11 -Wall -g -lm -fprofile-arcs -ftest-coverage -lgcov -fopenmp
CC=gcc
LIBSDIR=../library/

default: ex9
all: ex9 

.PHONY: clean all deps default run valgrind ex9 check 

ex9: deps
	$(CC) -o ex9 steiner_modified.c $(LIBSDIR)*.c $(CFLAGS) -I$(LIBSDIR)

deps: 
	$(MAKE) -C $(LIBSDIR) clean
	$(MAKE) -C $(LIBSDIR)

run: ex9
	./ex9 ../../data/b15.gph

valgrind: ex9
	valgrind ./ex9 ../../data/b15.gph

check:
	cppcheck . 2> result.xml

coverage: run
	gcov *.c $(LIBSDIR)*.c -o . 
	lcov --capture --directory . --output-file coverage.info
	genhtml coverage.info --output-directory coverage

clean:
	rm -rf *.o ex9 *.gcno *.gcda coverage*
