FILE=world666.gph
THREADS=1
STARTS=100

compile:
	g++ -g -fopenmp -Wall ex10.cpp dijkstra.h dijkstra.cpp steinCheck.h steinCheck.cpp findSteiner.h findSteiner.cpp -o ex10 -std=c++11 -lboost_system -lboost_timer -O3

BIG:
	g++ -g -fopenmp -Wall ex10.cpp dijkstra.h dijkstra.cpp steinCheck.h steinCheck.cpp findSteiner.h findSteiner.cpp -o ex10 -std=c++11 -lboost_system -lboost_timer -O3 -D BIG

test:
		g++ -g -fopenmp -Wall ex10.cpp dijkstra.h dijkstra.cpp steinCheck.h steinCheck.cpp findSteiner.h findSteiner.cpp -o ex10 -std=c++11 -lboost_system -lboost_timer -O3
		./ex10  $(FILE) $(STARTS) $(THREADS)
testBIG:`
	g++ -g -fopenmp -Wall ex10.cpp dijkstra.h dijkstra.cpp steinCheck.h steinCheck.cpp findSteiner.h findSteiner.cpp -o ex10 -std=c++11 -lboost_system -lboost_timer -O3 -D BIG
	./ex10  $(FILE) $(STARTS) $(THREADS)
