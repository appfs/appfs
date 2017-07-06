CFLAGS= -std=c11 -Wall -g -lm -fprofile-arcs -ftest-coverage -lgcov
CC=gcc
OBJ=graph.o misc.o heap.o
HEADERS=graph.h misc.c heap.c

default: ex8
all: ex5 ex8

.PHONY: clean all default run5 run8 valgrind5 valgrind8 doc ex5 ex8 check5 check8 coverage5 coverage8

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

ex5: $(OBJ)
	$(CC) -o ex5 longest_shortest.c $^ $(CFLAGS)

ex8: $(OBJ)
	$(CC) -o ex8 steiner.c $^ $(CFLAGS)

run5: ex5
	./ex5 ../data/small.gph

valgrind5: ex5
	valgrind ./ex5 ../data/small.gph

check5: 
	cppcheck graph.c graph.h longest_shortest.c 2> cppcheck5.txt

coverage5: run5
	gcov longest_shortest.c graph.c

run8: ex8
	./ex8 ../data/small.gph

valgrind8: ex8
	valgrind ./ex8 ../data/small.gph

coverage8: run8
	gcov steiner.gph graph.c

check8:
	cppcheck graph.c graph.h steiner.c 2> cppcheck8.txt

clean:
	rm -f *.o ex5 ex8 *.c.gcov *.gcno *.gcda
	rm -rf html
	rm -rf latex

doc:
	doxygen dox-conf
