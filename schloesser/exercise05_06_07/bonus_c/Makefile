CFLAGS= -std=c11 -Wall -g
CC=gcc
OBJ=longest_shortest.o graph.o
HEADERS=graph.h

default: ex8
all: default

.PHONY: clean all default run $(TARGET) valgrind doc

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

ex5: $(OBJ)
	$(CC) $(CFLAGS) -o ex5 $^

ex8: $(OBJ)
	$(CC) $(CFLAGS) -o ex8 $^

run: ex8
	./ex8 ../data/small.gph

valgrind: ex8
	valgrind ./ex8 ../data/small.gph

clean:
	rm -f *.o main ex5 ex8
	rm -r html
	rm -r latex

doc:
	doxygen dox-conf
