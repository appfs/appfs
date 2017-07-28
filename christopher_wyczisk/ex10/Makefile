doc:
	doxygen config.dxy
	
coverage:
	g++ -O3 -std=c++0x source/kommunikator.cpp source/dijkstra.cpp source/graph.cpp source/steiner.cpp source/main.cpp -o main -lboost_timer -fopenmp -lboost_system
	./main 1 ../test.gph
	lcov --capture --directory ../ --output-file coverage.info
	genhtml coverage.info --output-directory out

check:
	cppcheck source/dijkstra.cpp
	cppcheck source/steiner.cpp
	cppcheck source/kommunikator.cpp
	cppcheck source/main.cpp
	
run:
	g++ -O3 -std=c++0x source/kommunikator.cpp source/dijkstra.cpp source/graph.cpp source/steiner.cpp source/main.cpp -o main -lboost_timer -fopenmp -lboost_system
	./main 1 ../test.gph