run:
	python ex2.py data/measured-1.0.0.2017-02-03.b0050c5c8deb1db59c7b2644414b079d.xml.gz data/measured-1-1-0.xsd

doc:
	doxygen dox-conf

coverage:
	coverage3 run --omit=../appfs/* ex2.py data/measured-1.0.0.2017-02-03.b0050c5c8deb1db59c7b2644414b079d.xml.gz data/measured-1-1-0.xsd
	coverage3 report -m

check:
	pylint ex2.py --rcfile=pyl-conf

