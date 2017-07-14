doc:
	doxygen config.dxy
	
coverage:
	g++ -std=c++14 --coverage ex5.cpp -o ex5 -lboost_system -lboost_timer
	./ex5 ../acycpos-1.gph
	lcov --capture --directory ../ --output-file coverage.info
	genhtml coverage.info --output-directory out

check:
	cppcheck ex5.cpp
	
run:
	g++ -std=c++14 ex5.cpp -o ex5 -lboost_system -lboost_timer
	./ex5 ../acycpos-1.gph