build:
	g++ -O3 -std=c++0x src/ex10.cpp src/Dijkstra.cpp src/Steiner.cpp src/GraphReader.cpp -o ex10 -lboost_program_options -lboost_timer
	
doc:
	doxygen doxyDocu
	
coverage:
	g++ --coverage -std=c++0x src/ex10.cpp src/Dijkstra.cpp src/Steiner.cpp src/GraphReader.cpp -o ex10 -lboost_program_options -lboost_timer
	./ex10 ../b15.gph
	lcov --capture --directory ../ --output-file coverage.info
	genhtml coverage.info --output-directory out
	firefox out/index.html

check:
	cppcheck ex10.cpp
	
run:
	g++ -O3 -std=c++0x src/ex10.cpp src/Dijkstra.cpp src/Steiner.cpp src/GraphReader.cpp -o ex10 -lboost_program_options -lboost_timer
	./ex10 ../b15.gph
	
debug:
	g++ -g -std=c++0x src/ex10.cpp src/Dijkstra.cpp src/Steiner.cpp src/GraphReader.cpp -o ex10 -lboost_program_options -lboost_timer
	
test:
	g++ -O3 -std=c++0x src/ex10.cpp src/Dijkstra.cpp src/Steiner.cpp src/GraphReader.cpp -o ex10 -lboost_program_options -lboost_timer
	./ex10 ${FILE}
