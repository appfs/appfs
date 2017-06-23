run:
	python3 ex5_standalone.py ../data/b15.gph

doc:
	doxygen dox-conf

coverage:
	coverage3 run --omit=../appfs/* ex5_standalone.py ../data/b15.gph
	coverage3 report -m

coverage2:
	python3-coverage run --omit=../appfs/* ex5_standalone.py ../data/b15.gph
	python3-coverage report -m

check:
	pylint ex5_standalone.py --rcfile=pyl-conf

check2:
	pylint3 ex5_standalone.py --rcfile=pyl-conf

