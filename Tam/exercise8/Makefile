pyscript = ex8.py

	
coverage: 
	coverage run $(pyscript) ${arg1}
	coverage report
	coverage html
	open htmlcov/ex8_py.html 


check:
	@echo “TBD”


run: 
	python3 $(pyscript) ${arg1}

doc:
	yes | cp -rf $(pyscript) ex8documentation/
	cd ex8documentation/; make clean; make html; open _build/html/Code_Documentation.html; open _build/html/index.html; cd ../
	@echo “PLEASE NOTE: There is an error with generating documentation from the python script. Please refer to the index.html file for more info”


help:
	@echo “run”
	@echo “		- Run ex8.py on a specific mode and graph”
	@echo “		- specify graphfile as arg1
	@echo “		- for example… make run arg1=b15.gph”
	@echo “coverage”
	@echo “		- Produce coverage results of the program”
	@echo “		- execution example… make coverage arg1=b15.gph”
	@echo “doc”
	@echo “		- Produces and opens up an html representing documentation of the program”