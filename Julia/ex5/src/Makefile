doc:
	doxygen doxyDocu
	
coverage:
	g++ --coverage ex5.cpp -o ex5
	./ex5 ../acycpos-1.gph
	lcov --capture --directory ../ --output-file coverage.info
	genhtml coverage.info --output-directory out
	firefox out/index.html

check:
	cppcheck ex5.cpp
	
run:
	g++ ex5.cpp -o ex5
	./ex5 ../acycpos-1.gph
