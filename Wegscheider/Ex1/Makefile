doc: ex1.cpp
		doxygen -g
		doxygen Doxyfile

coverage: ex1.cpp
		g++ -O0 -g -Wall -LDFLAGS ex1.cpp -o ex1 --coverage
		./ex1 ex1.dat
		lcov --capture --directory ex1.gcno --output-file coverage.info
		genhtml coverage.info --output-directory coverage
		
check: ex1.cpp
		cppcheck --enable=all ex1.cpp