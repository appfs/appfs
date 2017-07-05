CFLAGS= -std=c11 -Wall -g -lm
CC=gcc
OBJ=graph.o
HEADERS=graph.h

default: ex8
all: ex5 ex8

.PHONY: clean all default run valgrind doc ex5 ex8

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

ex5: $(OBJ)
	$(CC) $(CFLAGS) -o ex5 longest_shortest.c $^

ex8: $(OBJ)
	$(CC) -o ex8 steiner.c $^ $(CFLAGS)

run5: ex5
	./ex5 ../data/small.gph

valgrind5: ex5
	valgrind ./ex5 ../data/small.gph

run8: ex8
	./ex8 ../data/steiner.gph

valgrind8: ex8
	valgrind ./ex8 ../data/steiner.gph

clean:
	rm -f *.o ex5 ex8
	rm -rf html
	rm -rf latex

doc:
	doxygen dox-conf
