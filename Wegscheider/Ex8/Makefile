ex8: ex8.o ShortestPathHeuristic.o MyDijkstra.o GraphParser.o
	g++ -std=c++11 -o  ex8  ex8.o ShortestPathHeuristic.o MyDijkstra.o GraphParser.o -lboost_timer -lboost_program_options

ex8.o: ex8.cpp ShortestPathHeuristic.o MyDijkstra.o GraphParser.o
	g++ -std=c++11 -O3 -Wall -c -fmessage-length=0 -o ex8.o ex8.cpp -lboost_timer -lboost_program_options

GraphParser.o: GraphParser.h GraphParser.cpp
	g++ -std=c++11 -O3 -Wall -c -fmessage-length=0 -o GraphParser.o GraphParser.cpp -lboost_timer -lboost_program_options

MyDijkstra.o: MyDijkstra.h MyDijkstra.cpp
	g++ -std=c++11 -O3 -Wall -c -fmessage-length=0 -o MyDijkstra.o MyDijkstra.cpp -lboost_timer -lboost_program_options

ShortestPathHeuristic.o: ShortestPathHeuristic.h ShortestPathHeuristic.cpp MyDijkstra.o
	g++ -std=c++11 -O3 -Wall -c -fmessage-length=0 -o ShortestPathHeuristic.o ShortestPathHeuristic.cpp -lboost_timer -lboost_program_options

doc: ex8.cpp
		doxygen -g
		doxygen Doxyfile
		
coverage: ex8
		g++ -std=c++11 -std=c++11 -O3 -Wall ex8.cpp -o ex8 ShortestPathHeuristic.o MyDijkstra.o GraphParser.o -lboost_timer -lboost_program_options --coverage
		./ex8 world666.gph
		lcov --capture --directory ex8.gcno --output-file coverage.info
		genhtml coverage.info --output-directory coverage
		
check: ex8.cpp
		cppcheck --enable=all ex8.cpp
		