.DEFAULT_GOAL := run
.PHONY: default
default: run ;

run:
	python3 ex5/ex5.py ../data/small.gph

doc:
	doxygen dox-conf

coverage:
	coverage3 run --omit=../appfs/* ex5/ex5.py ../data/small.gph
	coverage3 report -m

coverage2:
	python3-coverage run --omit=../appfs/* ex5/ex5.py ../data/small.gph
	python3-coverage report -m

check:
	pylint ex5/* --rcfile=pyl-conf

check2:
	pylint3 ex5/ex5.py --rcfile=pyl-conf

