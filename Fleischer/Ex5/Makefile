ex5: ex5.c
	gcc -std=c11 -O3 -g -Wall -Wextra -Wpedantic -LDFLAGS ex5.c -o ex5 -lm

doc: ex5.c
	doxygen -s -g ex5.docu
	doxygen ex5.docu	
	
coverage: ex5.c
	gcc -std=c11 -O3 -g -Wall -Wextra -Wpedantic -LDFLAGS ex5.c -o ex5 -lm  --coverage
	./ex5 world666.gph
	lcov --capture --directory ex5.gcno --output-file coverage.info
	genhtml coverage.info --output-directory coverage
	
check: ex5.c
	cppcheck --enable=all ex5.c
	
			
