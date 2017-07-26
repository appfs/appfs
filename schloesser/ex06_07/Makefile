.DEFAULT_GOAL := run
.PHONY: default
default: run ;

run:
	python3 ex6 ../../data/b15.gph

doc:
	doxygen dox-conf

coverage:
	coverage3 run --omit=../appfs/* ex6 ../../data/b15.gph
	coverage3 report -m

coverage2:
	python3-coverage run --omit=../appfs/* ex6 ../../data/b15.gph
	python3-coverage report -m

check:
	pylint * --rcfile=pyl-conf

check2:
	pylint3 ex6 --rcfile=pyl-conf

