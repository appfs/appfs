doc:
	doxygen doxy
	
coverage:
	@echo 'Compiling'
	cd ../../appfs_coverage/
	make
	@echo 'Running Program'
	ex2 measured-1.0.0.2017-02-03.b0050c5c8deb1db59c7b2644414b079d.xml
	@echo 'Analysing Coverage Data'
	lcov --capture --directory ./ --output-file coverage.info
	genhtml coverage.info --output-directory out
	cd ../src/c/
	@echo 'Open Results'
	firefox out/index.html
	
	
check:
	cppcheck ../
	