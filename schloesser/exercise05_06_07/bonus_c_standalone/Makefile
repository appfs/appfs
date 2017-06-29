CFLAGS= -std=c11 -Wall -g
CC=gcc
OBJ=
HEADERS=
TARGET=main

default: $(TARGET)
all: default

.PHONY: clean all default run $(TARGET)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET):
	$(CC) $(CFLAGS) main.c -o $(TARGET)

run: $(TARGET)
	./main ../data/small.gph

clean:
	rm -f *.o main

