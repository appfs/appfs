doc:
	doxygen doxy
	
coverage:
	@echo 'Compiling'
	g++ -std=c++0x -O0 -g3 -Wall --coverage ex5.cpp -o ex2
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
	
compile:
	g++ -std=c++0x -O3 -Wall ex5.cpp -o ex2
	
run:
	make compile
	./ex5 phard-1
	
	