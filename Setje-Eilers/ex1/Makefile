default: run

run: 
	python3 ex1.py ex1-100.dat

doc: 
	doxygen ex1_doxyfile

coverage: 
	coverage run ex1.py ex1-100.dat
	coverage report

check: 
	pylint ex1.py
