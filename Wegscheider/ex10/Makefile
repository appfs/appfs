compile: main.o SteinerTreeHeuristic.o PrimeNumbers.o GraphParser.o
	g++ -std=c++11 -fopenmp -o ex10 main.o SteinerTreeHeuristic.o PrimeNumbers.o GraphParser.o -lboost_system -lboost_timer -lboost_program_options

main.o: main.cpp SteinerTreeHeuristic.o PrimeNumbers.o GraphParser.o
	g++ -std=c++11 -O3 -Wall -c -fopenmp -fmessage-length=0 -o main.o main.cpp -lboost_system -lboost_timer -lboost_program_options 

GraphParser.o: GraphParser.h GraphParser.cpp
	g++ -std=c++11 -O3 -Wall -c -fmessage-length=0 -o GraphParser.o GraphParser.cpp

PrimeNumbers.o: PrimeNumbers.h PrimeNumbers.cpp
	g++ -std=c++11 -O3 -Wall -c -fmessage-length=0 -o PrimeNumbers.o PrimeNumbers.cpp

SteinerTreeHeuristic.o: SteinerTreeHeuristic.h SteinerTreeHeuristic.cpp
	g++ -std=c++11 -O3 -Wall -c -fopenmp -fmessage-length=0 -o SteinerTreeHeuristic.o SteinerTreeHeuristic.cpp

doc:
		doxygen -g
		doxygen Doxyfile
		
coverage:
		g++ -std=c++11 -std=c++11 -O3 -Wall main.cpp -o ex10 SteinerTreeHeuristic.o PrimeNumbers.o GraphParser.o -lboost_timer -lboost_program_options --coverage
		./ex10 world666.gph
		lcov --capture --directory main.gcno --output-file coverage.info
		genhtml coverage.info --output-directory coverage
		
check:
		cppcheck --enable=all main.cpp
		
