doc:
	doxygen config.dxy
	
coverage:
	python ex2.py example.measured-1-1-0.xml measured-1-1-0.xsd result.csv
	lcov --capture --directory ../ --output-file coverage.info
	genhtml coverage.info --output-directory out

check:
	cppcheck ex2.py
	
run:
	 python ex2.py example.measured-1-1-0.xml measured-1-1-0.xsd result.csv