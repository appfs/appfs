doc:
	doxygen doxyDocu
	
coverage:
	g++ --coverage ex2.cpp -o ex2
	./ex2 ../measured.xml
	lcov --capture --directory ../ --output-file coverage.info

check:
	cppcheck ex2.cpp
	cppcheck DocumentErrorHandler.cpp
	cppcheck DocumentErrorHandler.h
	cppcheck GasNode.cpp
	cppcheck GasNode.h