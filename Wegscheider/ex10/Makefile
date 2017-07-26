ex10: ex10.o SteinerTreeHeuristic.o PrimeNumbers.o GraphParser.o
	g++ -std=c++11 -fopenmp -o ex10 ex10.o SteinerTreeHeuristic.o PrimeNumbers.o GraphParser.o -lboost_system -lboost_timer -lboost_program_options

ex10.o: ex10.cpp SteinerTreeHeuristic.o PrimeNumbers.o GraphParser.o
	g++ -march=native -std=c++11 -O3 -pedantic -Wall -c -fopenmp -fmessage-length=0 -o ex10.o ex10.cpp 

GraphParser.o: GraphParser.h GraphParser.cpp
	g++ -march=native -std=c++11 -O3 -pedantic -Wall -c -fmessage-length=0 -o GraphParser.o GraphParser.cpp

PrimeNumbers.o: PrimeNumbers.h PrimeNumbers.cpp
	g++ -march=native -std=c++11 -O3 -pedantic -Wall -c -fmessage-length=0 -o PrimeNumbers.o PrimeNumbers.cpp

SteinerTreeHeuristic.o: SteinerTreeHeuristic.h SteinerTreeHeuristic.cpp
	g++ -march=native -std=c++11 -O3 -pedantic -Wall -c -fmessage-length=0 -o SteinerTreeHeuristic.o SteinerTreeHeuristic.cpp


doc:
		doxygen -g
		doxygen Doxyfile
		
		
coverage:
		g++ -std=c++11 -g -O0 -c -fmessage-length=0 -o GraphParser.o GraphParser.cpp --coverage
		g++ -std=c++11 -g -O0 -c -fmessage-length=0 -o PrimeNumbers.o PrimeNumbers.cpp --coverage
		g++ -std=c++11 -g -O0 -c -fmessage-length=0 -o SteinerTreeHeuristic.o SteinerTreeHeuristic.cpp --coverage
		g++ -std=c++11 -g -O0 -c -fopenmp -fmessage-length=0 -o ex10.o ex10.cpp --coverage
		g++ -std=c++11 -fopenmp -o ex10 ex10.o SteinerTreeHeuristic.o PrimeNumbers.o GraphParser.o --coverage -lboost_system -lboost_timer -lboost_program_options
		./ex10 ../../SP/b15.gph 10 -s
		lcov --capture --directory . --output-file coverage.info
		genhtml coverage.info --output-directory cov
		rm *.o
		rm ex10
		
		
check:
		cppcheck --enable=all ex10.cpp
		
		
test: ex10
	@./ex10 ../../SP/$(FILE) 100 -t $(THREADS)
		
