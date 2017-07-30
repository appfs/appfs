src = $(wildcard *.c)
obj = $(src:.c=.o)

LDFLAGS = -Wall -lm
CFLAGS = -std=c99 -O2

ex5: $(obj)
		$(CC) -g -o $@ $^ $(LDFLAGS) 

.PHONY: clean doxygen

clean:
	rm -f *.o

doc:
	doxygen doxygen_config
