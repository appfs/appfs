run:
	python ex1.py resources/ex1/test5.dat

docs:
	@doxygen ../../doc/doxygen.cfg

check:
	pylint ex2.py --rcfile=../../doc/pylint.cfg

coverage:
	python3-coverage run
	python3-coverage report -m