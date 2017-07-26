run:
	python ex1 ../../data/testdata
	python ex1 ../../data/cleandata
	python ex1 ../../data/longdata

doc:
	doxygen dox-conf

coverage:
	python3-coverage run --omit=../appfs/* ex1 ../../data/testdata
	python3-coverage report -m

check:
	pylint3 ex1 --rcfile=pyl-conf

