# Exercise 2
* compile: `g++ -std=c++11 -O3 ex2.cxx -o ex2 -lxerces-c`
* run: `./ex2 <xml filepath> [-V]`
* include `[-V]` flag to enable schema validation
* **ex2.cxx** uses the Xerces-C library for XML parsing. Accepts XML filepath as argument. Prints output to csv.txt. `[-V]` flag determines if XML file is to be validated. Namespace-location pair of external schema is fixed as `"http://gaslab.zib.de/kwpt/measured ./measured-1-1-0.xsd"`.
* Validation errors for given xml-xsd:
    * `line #2 column #1081: value '1.0.0' does not match regular expression facet '1\.1\.0'.`
    * `line #2 column #1081: attribute 'validity' is not declared for element 'measured'.` 
