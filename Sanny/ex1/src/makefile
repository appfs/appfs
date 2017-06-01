doc:
	doxygen doxy
	
coverage:
	@echo 'Compiling'
	g++ -std=c++0x -O0 -g3 -Wall --coverage ex1.cpp -o ex1
	@echo 'Running'
	./ex1 measured-1.0.0.2017-02-03.b0050c5c8deb1db59c7b2644414b079d.xml
	@echo 'Analysing Coverage Data'
	lcov --capture --directory ./ --output-file coverage.info
	genhtml coverage.info --output-directory out
	@echo 'Open Results'
	firefox out/index.html
	
	
check:
	cppcheck ../
	