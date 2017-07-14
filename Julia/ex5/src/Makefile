build:
	g++ -O3 -std=c++0x ex5.cpp dijkstra.cpp -o ex5 -lboost_timer

doc:
	doxygen doxyDocu
	
coverage:
	g++ --coverage -O3 -std=c++0x ex5.cpp dijkstra.cpp -o ex5 -lboost_timer
	./ex5 -m1 ../acycpos-1.gph
	lcov --capture --directory ../ --output-file coverage.info
	genhtml coverage.info --output-directory out
	firefox out/index.html

check:
	cppcheck ex5.cpp
	
run:
	g++ -O3 -std=c++0x ex5.cpp dijkstra.cpp -o ex5 -lboost_timer
	./ex5 -m1 ../acycpos-1.gph
	
debug:
	g++ -g -std=c++0x ex5.cpp dijkstra.cpp -o ex5 -lboost_timer
