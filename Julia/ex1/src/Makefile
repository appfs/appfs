doc:
	doxygen doxyDocu
	
coverage:
	g++ --coverage ex1.cpp -o ex1
	./ex1 ../ex100.dat
	lcov --capture --directory ../ --output-file coverage.info
	genhtml coverage.info --output-directory out
	firefox out/index.html

check:
	cppcheck ex1.cpp

run:
	g++ ex1.cpp -o ex1
	./ex1 ../ex100.dat