run:
	python ex2 ../../data/measured-1.0.0.2017-02-03.b0050c5c8deb1db59c7b2644414b079d.xml.gz data/measured-1-1-0.xsd

doc:
	doxygen dox-conf

coverage:
	python3-coverage run --omit=../appfs/* ex2 ../../data/measured-1.0.0.2017-02-03.b0050c5c8deb1db59c7b2644414b079d.xml.gz data/measured-1-1-0.xsd
	python3-coverage report -m

check:
	pylint3 ex2 --rcfile=pyl-conf

