doc:
	doxygen config.dxy
	
coverage:
	g++ -O3 -std=c++0x source/dijkstra.cpp source/steiner.cpp source/kommunikator.cpp source/main.cpp -o main -lboost_program_options
	./main --pfad ../../test.gph --start 1 2 3
	lcov --capture --directory ../ --output-file coverage.info
	genhtml coverage.info --output-directory out

check:
	cppcheck source/dijkstra.cpp
	cppcheck source/steiner.cpp
	cppcheck source/kommunikator.cpp
	cppcheck source/main.cpp
	
run:
	g++ -O3 -std=c++0x source/dijkstra.cpp source/steiner.cpp source/kommunikator.cpp source/main.cpp -o main -lboost_program_options
	./main --pfad ../../test.gph --start 1 2 3