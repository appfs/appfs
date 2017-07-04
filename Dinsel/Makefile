doc: FORCE
	cd doc && doxygen doxydoc.conf
	xdg-open doc/html/index.html

ex1: FORCE
	time ex1/ex1.py ex1/data/ex10k.dat

ex2: FORCE
	time ex2/ex2.py -v ex2/data/measured-1.0.0.2017-02-03.b0050c5c8deb1db59c7b2644414b079d.xml ex2/data/measured-1-1-0.xsd

ex3: FORCE
	time ex3/ex3.py

ex5: FORCE
	# time ex5/ex5.py data/b15.gph
	@echo "this does not work unfortunately.. will work on that"

ex6: FORCE
	@echo "not yet implemented..."

coverage: FORCE
	@echo "coverage for ex1.py"
	coverage run --include=ex1/ex1.py ex1/ex1.py ex1/data/ex10k.dat
	mv .coverage ex1/.coverage.ex1

	@echo "coverage for ex2"
	coverage run --include=ex2/ex2.py ex2/ex2.py ex2/data/measured-1.0.0.2017-02-03.b0050c5c8deb1db59c7b2644414b079d.xml ex2/data/measured-1-1-0.xsd
	mv .coverage ex2/.coverage.ex2

	@echo "coverage for ex3"
	coverage run --include=ex3/ex3.py ex3/ex3.py
	mv .coverage ex3/.coverage.ex3

	@echo "coverage for ex5"
	coverage run --include=ex5/ex5.py ex5/ex5.py ex5/data/b15.gph
	mv .coverage ex5/.coverage.ex5


	coverage combine ex1/.coverage.ex1 ex2/.coverage.ex2 ex3/.coverage.ex3 ex5/.coverage.ex5
	coverage html
	xdg-open htmlcov/index.html

check:
	@echo "not yet implemented..."

all:
	make coverage
	make doc

FORCE:
