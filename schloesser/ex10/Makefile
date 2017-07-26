CFLAGS= -std=gnu11 -Wall -g -lm -fprofile-arcs -ftest-coverage -lgcov -fopenmp 
CC=gcc
LIBSDIR=../library/
MAKEFLAGS+=--silent

default: ex10
all: ex10 

.PHONY: clean all deps default run valgrind ex10 check 

ex10: deps
	$(CC) -o ex10 steiner_modified_final.c $(LIBSDIR)*.c $(CFLAGS) -I$(LIBSDIR)

deps: 
	$(MAKE) -C $(LIBSDIR) clean
	$(MAKE) -C $(LIBSDIR)

run: ex10
	./ex10 ../../data/b15.gph 10

test: ex10 
ifeq ($(THREADS),)
	./ex10 $(FILE) 100 -s
else
	OMP_THREAD_LIMIT=$(THREADS) \
	OMP_NUM_THREADS=$(THREADS) \
	./ex10 $(FILE) 100 -s
endif

valgrind: ex10
	valgrind ./ex10 ../../data/b15.gph 100

check:
	cppcheck . 2> result.xml 

coverage: run
	gcov *.c $(LIBSDIR)*.c -o . 
	lcov --capture --directory . --output-file coverage.info
	genhtml coverage.info --output-directory coverage

clean:
	rm -rf *.o ex10 *.gcno *.gcda coverage* 
