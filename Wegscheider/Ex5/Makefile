doc: ex5.cpp
		doxygen -g
		doxygen Doxyfile
		
coverage: ex5.cpp
		g++ -O3 -c -Wall -LDFLAGS ex5.cpp -o ex5 --coverage
		./ex5 i640-345.gph
		lcov --capture --directory ex5.gcno --output-file coverage.info
		genhtml coverage.info --output-directory coverage
		
check: ex5.cpp
		cppcheck --enable=all ex5.cpp