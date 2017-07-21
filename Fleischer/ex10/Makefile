STARTS=100
SOURCES := $(shell find $(SOURCEDIR) -samefile 'SP')

doc: ex10.c
	doxygen -s -g ex10.docu
	doxygen ex10.docu	
	
coverage: ex10.c
	gcc -std=c11 -O3 -g -Wall -Wextra -Wpedantic -LDFLAGS -fopenmp ex10.c -o ex10 -lm  --coverage
	./ex10 world666.gph 100
	lcov --capture --directory ex10.gcno --output-file coverage.info
	genhtml coverage.info --output-directory coverage
	
check: ex10.c
	cppcheck --enable=all ex10.c
	
test:	
	gcc -std=c11 -O3 -Wall -Wextra -Wpedantic -LDFLAGS -fopenmp ex10.c -o ex10 -lm	
	./ex10 ../$(SOURCES)/SP/${FILE} $(STARTS) ${THREADS} 
