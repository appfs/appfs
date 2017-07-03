compile:
	g++ -std=c++0x -O3 -Wall ex5.cpp Dijkstra.cpp -o ex5 -lboost_system -lboost_timer -lboost_program_options

doc:
	doxygen doxy
	
coverage:
	@echo 'Compiling for Debugging'
	g++ -std=c++0x -O0 -g3 -Wall --coverage ex5.cpp Dijkstra.cpp -o ex5 -lboost_system -lboost_timer -boost_program_options
	@echo 'Running Program'
	./ex5 phard-1
	@echo 'Analysing Coverage Data'
	lcov --capture --directory ./ --output-file coverage.info
	genhtml coverage.info --output-directory out
	cd ../src/c/
	@echo 'Open Results'
	firefox out/index.html
	
check:
	cppcheck ../
	
run:
	make compile
	./ex5 phard-1
	
	