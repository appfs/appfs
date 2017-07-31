src = $(wildcard *.c)
obj = $(src:.c=.o)

ifneq ($(strip $(THREADS)),)
CFLAGS = -std=c99 -O3 -fopenmp -Wall -pedantic -DTHREADS=$(THREADS) -march=native
LDFLAGS = -lm
else
CFLAGS = -std=c99 -O3 -fopenmp -Wall -pedantic -march=native
LDFLAGS =-lm
endif

ex10: $(obj)
	$(CC) $(CFLAGS) -g -o $@ $^ $(LDFLAGS) 
ifneq ($(strip $(FILE)),)
	./ex10 $(FILE)
endif

.PHONY: 
	clean doc

clean:
	rm -f *.o

doc:
	doxygen doxygen_config
