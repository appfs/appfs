src = $(wildcard *.c)
obj = $(src:.c=.o)

LDFLAGS = -std=gnu99 -lm

ex1: $(obj)
		$(CC) -g -o $@ $^ $(LDFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
