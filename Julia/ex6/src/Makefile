build:
	g++ -O3 -std=c++0x ex6.cpp dijkstra.cpp Steiner.cpp -o ex6 -lboost_timer

doc:
	doxygen doxyDocu
	
coverage:
	g++ --coverage -O3 -std=c++0x ex6.cpp dijkstra.cpp Steiner.cpp -o ex6 -lboost_timer
	./ex6 ../acycpos-1.gph 1
	lcov --capture --directory ../ --output-file coverage.info
	genhtml coverage.info --output-directory out
	firefox out/index.html

check:
	cppcheck ex6.cpp
	
run:
	g++ -O3 -std=c++0x ex6.cpp dijkstra.cpp Steiner.cpp -o ex6 -lboost_timer
	./ex6 ../acycpos-1.gph 1
	
debug:
	g++ -g -std=c++0x ex6.cpp dijkstra.cpp Steiner.cpp -o ex6 -lboost_timer
