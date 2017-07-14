run:
	python ex1.py test/testdata
	python ex1.py test/cleandata
	python ex1.py test/longdata

doc:
	doxygen dox-conf

coverage:
	python3-coverage run --omit=../appfs/* ex1.py test/testdata
	python3-coverage report -m

check:
	pylint3 ex1.py --rcfile=pyl-conf

