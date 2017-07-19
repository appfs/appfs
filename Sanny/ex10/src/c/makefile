FOLDER=../../SP
FILE=world666
THREADS=8

compile: ex10.o Dijkstra.o Steiner.o GraphChecker.o
	@echo 'Compiling'
	g++ -std=c++0x -O3 -Wall ex10.o Dijkstra.o Steiner.o GraphChecker.o -o ex10 -lboost_system -lboost_timer -lboost_program_options -fopenmp
	@echo done

test: compile
	./ex10 -t $(THREADS) $(FOLDER)/$(FILE)

doc:
	doxygen doxy
	
coverage:
	@echo 'Compiling for Debugging'
	g++ -std=c++0x -O0 -g3 -Wall --coverage ex10.cpp Dijkstra.cpp Steiner.cpp GraphChecker.cpp -o Ex10 -lboost_system -lboost_timer -boost_program_options
	@echo 'Running Program'
	./ex10 -sn 2 $(FOLDER)/$(FILE)
	@echo 'Analysing Coverage Data'
	lcov --capture --directory ./ --output-file coverage.info
	genhtml coverage.info --output-directory out
	cd ../src/c/
	@echo 'Open Results'
	firefox out/index.html
	
check:
	cppcheck ./
	
run: compile
	./ex10 $(FOLDER)/$(FILE)
	
ex10.o: ex10.cpp
	 g++ -std=c++0x -O3 -Wall -c ex10.cpp -lboost_system -lboost_timer -lboost_program_options -fopenmp
	
Dijkstra.o: Dijkstra.cpp
	 g++ -std=c++0x -O3 -Wall -c Dijkstra.cpp -fopenmp
	 
Steiner.o: Steiner.cpp
	 g++ -std=c++0x -O3 -Wall -c Steiner.cpp -fopenmp
	 
GraphChecker.o: GraphChecker.cpp
	 g++ -std=c++0x -O3 -Wall -c GraphChecker.cpp -fopenmp
	
