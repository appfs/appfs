doc:
	doxygen config.dxy
	
coverage:
	g++ -std=c++14 --coverage ex3.c -o ex3
	./ex3
	lcov --capture --directory ../ --output-file coverage.info
	genhtml coverage.info --output-directory out

check:
	cppcheck ex3.c
	
run:
	g++ -std=c++14 ex3.c -o ex3
	./ex3