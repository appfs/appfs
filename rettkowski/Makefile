doc: ex1.cpp
	doxygen -g ex1.doxygen.conf
	doxygen ex1.doxygen.conf
	
coverage: ex1.cpp
	g++ -std=c11 ex1.cpp -o ex1 -lm --coverage
	./ex1 ex1.dat
	lcov --capture --directory ex1.gcno --output-file ex1.coverage.info
	genhtml ex1.coverage.info --output-directory coverage
	
check: ex1.cpp
	cppcheck --enable=all ex1.cpp

build: ex1.cpp
	g++ -std=c++11 ex1.cpp -o ex1
	
run: ex1.cpp
	g++ -std=c++11 ex1.cpp -o ex1
	./ex1 ex1.dat
