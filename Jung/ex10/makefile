FILE=world666.gph
THREADS=1
STARTS=100

CXX = g++
FLAGS= -std=c++11 -Wall -o ex10
FAST= -march=native -O3
DBG=-g -O0
LIBS=-fopenmp -lboost_system -lboost_timer
SOURCE= ex10.cpp utils.h utils.cpp dijkstra.h dijkstra.cpp steinCheck.h steinCheck.cpp



compile:
		$(CXX) $(FLAGS) $(FAST) $(LIBS) $(SOURCE)

debug:
		$(CXX) $(FLAGS) $(DBG) $(LIBS) $(SOURCE)

test:	compile
		./ex10  $(FILE) $(STARTS) $(THREADS)

valgrind: debug
		valgrind -v --leak-check=full --show-leak-kinds=all --track-origins=yes --dsymutil=yes ./ex10 $(FILE) $(STARTS) $(THREADS)

check:
		cppcheck --enable=all --inconclusive --std=posix $(SOURCE)

doc:
		doxygen -g

coverage:
		$(CXX)  $(FLAGS) $(DBG) $(LIBS) $(SOURCE) --coverage
		./ex10  $(FILE) $(STARTS) $(THREADS) -s
		lcov --capture --directory . --output-file coverage.info 
		genhtml coverage.info --output-directory covHTML