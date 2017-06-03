doc:
	doxygen doxy
	
coverage:
	@echo 'Compiling'
	g++ -std=c++0x -O0 -g3 -Wall --coverage ex5.cpp -o ex2
	@echo 'Running Program'
	./ex5 
	@echo 'Analysing Coverage Data'
	lcov --capture --directory ./ --output-file coverage.info
	genhtml coverage.info --output-directory out
	cd ../src/c/
	@echo 'Open Results'
	firefox out/index.html
	
	
check:
	cppcheck ../
	