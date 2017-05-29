test:
	python ex1.py test/testdata
	python ex1.py test/cleandata
	python ex1.py test/longdata

doc:
	doxygen dox-conf

coverage:
	python3-coverage run --omit=/usr/* ex1.py test/testdata
	python3-coverage report -m

check:
	echo "please implement me"

