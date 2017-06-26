doc: ex1.c
	doxygen -s -g ex1.docu
	doxygen ex1.docu	
	
coverage: ex1.c
	gcc -std=c11 -O3 -g -Wall -Wextra -Wpedantic -LDFLAGS ex1.c -o ex1 -lm --coverage
	./ex1 ex1.dat
	lcov --capture --directory ex1.gcno --output-file coverage.info
	genhtml coverage.info --output-directory coverage
	
check: ex1.c
	cppcheck --enable=all ex1.c
	
			
