build:
	g++ -O3 -std=c++0x src/ex10.cpp src/DijkstraSolver.cpp src/SteinerSolver.cpp src/GraphReader.cpp src/TreeChecker.cpp -o ex10 -lboost_timer -fopenmp
	
doc:
	doxygen doxyDocu
	
coverage:
	g++ --coverage -std=c++0x src/ex10.cpp src/DijkstraSolver.cpp src/SteinerSolver.cpp src/GraphReader.cpp src/TreeChecker.cpp -o ex10 -lboost_timer -fopenmp
	./ex10 ../b15.gph
	lcov --capture --directory ../ --output-file coverage.info
	genhtml coverage.info --output-directory out
	firefox out/index.html

check:
	cppcheck ex10.cpp
	
run:
	g++ -O3 -std=c++0x src/ex10.cpp src/DijkstraSolver.cpp src/SteinerSolver.cpp src/GraphReader.cpp src/TreeChecker.cpp -o ex10 -lboost_timer -fopenmp
	./ex10 ../b15.gph
	
debug:
	g++ -g -std=c++0x src/ex10.cpp src/DijkstraSolver.cpp src/SteinerSolver.cpp src/GraphReader.cpp src/TreeChecker.cpp -o ex10 -lboost_timer -fopenmp
	
test:
	g++ -O3 -std=c++0x src/ex10.cpp src/DijkstraSolver.cpp src/SteinerSolver.cpp src/GraphReader.cpp src/TreeChecker.cpp -o ex10 -lboost_timer -fopenmp
	./ex10 ${THREADS} ../../${FILE}
