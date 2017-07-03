ex6: ex6.o
	g++ -std=c++11 -o ex6  ex6.o -lboost_timer -lboost_program_options

ex6.o: ex6.cpp
	g++ -std=c++11 -O3 -Wall -c -fmessage-length=0 -o ex6.o ex6.cpp 

doc: ex6.cpp
		doxygen -g
		doxygen Doxyfile
		
coverage: ex6.cpp
		g++ -std=c++11 -O3 -Wall ex6.cpp -o ex6 -lboost_timer -lboost_program_options --coverage
		./ex6 world666.gph --m1
		lcov --capture --directory ex6.gcno --output-file coverage.info
		genhtml coverage.info --output-directory coverage
		
check: ex6.cpp
		cppcheck --enable=all ex6.cpp
		