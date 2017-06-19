doc:
	doxygen doxyDocu
	
coverage:
	g++ --coverage ex2.cpp DocumentErrorHandler.cpp GasNode.cpp tinyxml2.cpp -o ex2 -lxerces-c
	./ex2 ../measured.xml
	lcov --capture --directory ../ --output-file coverage.info
	genhtml coverage.info --output-directory out
	firefox out/index.html

check:
	cppcheck ex2.cpp
	cppcheck DocumentErrorHandler.cpp
	cppcheck DocumentErrorHandler.h
	cppcheck GasNode.cpp
	cppcheck GasNode.h
	
run:
	g++ ex2.cpp DocumentErrorHandler.cpp GasNode.cpp tinyxml2.cpp -o ex2 -lxerces-c
	./ex2 ../measured.xml