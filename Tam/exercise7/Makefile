pyscript = ex7.py

	
coverage: 
	coverage run $(pyscript) ${arg1} ${arg2}
	coverage report


check:
	@echo “TBD”


run: 
	python3 $(pyscript) ${arg1} ${arg2}

doc:
	yes | cp -rf $(pyscript) ex7documentation/
	cd ex7documentation/; make clean; make html; open _build/html/Code_Documentation.html; cd ../

help:
	@echo “run”
	@echo “		- Run ex7.py on a specific mode and graph”
	@echo “		- specify mode in ‘arg1’ and graphfile in ‘arg2’”
	@echo “		- for example… make run arg1=-m1 arg2=b15.gph”
	@echo “coverage”
	@echo “		- Produce coverage results of the program”
	@echo “		- execution example… make coverage arg1=-m1 arg2=b15.gph”
	@echo “doc”
	@echo “		- Produces and opens up an html representing documentation of the program”