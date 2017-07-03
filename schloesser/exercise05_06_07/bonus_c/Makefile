CFLAGS= -std=c11 -Wall -g
CC=gcc
OBJ=main.o graph.o
HEADERS=graph.h
TARGET=main

default: $(TARGET)
all: default

.PHONY: clean all default run $(TARGET) valgrind doc

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $^

run: $(TARGET)
	./main ../data/small.gph

valgrind: $(TARGET)
	valgrind ./main ../data/small.gph

clean:
	rm -f *.o main
	rm -r html

doc:
	doxygen dox-conf
