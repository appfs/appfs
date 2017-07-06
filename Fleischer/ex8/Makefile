ex8: ex8.c
	gcc -std=c11 -O3 -g -Wall -Wextra -Wpedantic -LDFLAGS ex8.c -o ex8 -lm

doc: ex8.c
	doxygen -s -g ex8.docu
	doxygen ex8.docu	
	
coverage: ex8.c
	gcc -std=c11 -O3 -g -Wall -Wextra -Wpedantic -LDFLAGS ex8.c -o ex8 -lm  --coverage
	./ex8 world666.gph
	lcov --capture --directory ex8.gcno --output-file coverage.info
	genhtml coverage.info --output-directory coverage
	
check: ex8.c
	cppcheck --enable=all ex8.c
	
			
