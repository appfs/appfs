STARTS=100

doc: ex10.c Graph.h misc.h dataStruct.h
	doxygen -s -g ex10.docu
	doxygen ex10.docu
	
check: ex10.c
	cppcheck --enable=all ex10.c
	
debug: ex10.c	
	gcc -std=c11 -O3 -g -Wall -Wextra -Wpedantic -LDFLAGS -fopenmp ex10.c -o ex10 -lm
	gdb ex10
	
	
test:	ex10.o Graph.o dataStruct.o misc.o
	gcc -std=c11 -O3 -Wall -Wextra -Wpedantic -LDFLAGS -fopenmp ex10.o -o ex10 -lm	
	./ex10 ../SP/${FILE} $(STARTS) ${THREADS} 
	
valgrind: ex10.c
	gcc -std=c11 -O3 -g -Wall -Wextra -Wpedantic -LDFLAGS -fopenmp ex10.c -o ex10 -lm
	valgrind ./ex10 ../SP/world666.gph 10 1

ex10: ex10.o Graph.o dataStruct.o misc.o
	gcc -std=c11 -O3 -Wall -Wextra -Wpedantic -LDFLAGS -fopenmp ex10.o -o ex10 -lm

ex10.o: ex10.c Graph.c dataStruct.c misc.c Graph.h dataStruct.h misc.h
	gcc -std=c11 -O3 -Wall -Wextra -Wpedantic -LDFLAGS -fopenmp ex10.c -c ex10 -lm

