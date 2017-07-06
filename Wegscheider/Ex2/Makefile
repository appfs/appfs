doc: ex2.cpp
		doxygen -g
		doxygen Doxyfile

coverage: ex2.cpp
		g++ -O0 -g -Wall -LDFLAGS ex2.cpp -o ex2 --coverage
		./ex2 measured-1.0.0.2017-02-03.b0050c5c8deb1db59c7b2644414b079d.xml
		lcov --capture --directory ex2.gcno --output-file coverage.info
		genhtml coverage.info --output-directory coverage
		
check: ex2.cpp
		cppcheck --enable=all ex2.cpp