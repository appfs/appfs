CFLAGS= -std=c11 -Wall -g -lm -fprofile-arcs -ftest-coverage -lgcov
CC=gcc
OBJ=graph.o misc.o heap.o
HEADERS=graph.h misc.h heap.h

default: $(OBJ)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o *.c.gcov *.gcno *.gcda
	rm -fr latex
	rm -fr html

