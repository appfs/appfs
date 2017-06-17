doc:
	doxygen config.dxy
	
coverage:
	python ex1.py ex1.dat.
	lcov --capture --directory ../ --output-file coverage.info
	genhtml coverage.info --output-directory out

check:
	cppcheck ex1.py
	
run:
	python ex1.py ex1.dat.