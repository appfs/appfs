build:
	g++ -O3 -std=c++0x ex8.cpp dijkstra.cpp Steiner.cpp -o ex8
	
doc:
	doxygen doxyDocu
	
coverage:
	g++ --coverage -O3 -std=c++0x ex8.cpp dijkstra.cpp Steiner.cpp -o ex8
	./ex8 ../b15.gph 2
	lcov --capture --directory ../ --output-file coverage.info
	genhtml coverage.info --output-directory out
	firefox out/index.html

check:
	cppcheck ex8.cpp
	
run:
	g++ -O3 -std=c++0x ex8.cpp dijkstra.cpp Steiner.cpp -o ex8
	./ex8 ../b15.gph 2
	
debug:
	g++ -g -std=c++0x ex8.cpp dijkstra.cpp Steiner.cpp -o ex8
