doc: ex2.c
	doxygen -s -g ex2.docu
	doxygen ex2.docu	
	
coverage: ex2.c
	gcc -std=c11 -O3 -g -Wall -Wextra -Wpedantic -LDFLAGS ex2.c -o ex1 -lm --coverage
	./ex2 measured-1.0.0.2017-02-03.b0050c5c8deb1db59c7b2644414b079d.xml
	lcov --capture --directory ex2.gcno --output-file coverage.info
	genhtml coverage.info --output-directory coverage
	
check: ex2.c
	cppcheck --enable=all ex2.c