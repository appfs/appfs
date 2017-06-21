build:
	g++ -std=c++0x ex5.cpp dijkstra.cpp -o ex5 -lboost_timer -lboost_system

doc:
	doxygen doxyDocu
	
coverage:
	g++ --coverage -std=c++0x ex5.cpp dijkstra.cpp -o ex5 -lboost_timer -lboost_system
	./ex5 -m2 ../acycpos-1.gph
	lcov --capture --directory ../ --output-file coverage.info
	genhtml coverage.info --output-directory out
	firefox out/index.html

check:
	cppcheck ex5.cpp
	
run:
	g++ -std=c++0x ex5.cpp dijkstra.cpp -o ex5 -lboost_timer -lboost_system
	./ex5 -m2 ../acycpos-1.gph
