# Exercise 10
Calculate the best Steiner Tree for a given number of starting terminals (default is 100) and a given graph file.

* **Compile**: `g++ -std=c++14 -O3 -march=native -pedantic -Wall src/utils.o src/heap.o src/steiner.o src/checker.o src/ex10.cxx -o ex10 -lboost_timer -lboost_program_options -lboost_system [-fopenmp]`. Include  `-fopenmp` to enable parallelization.

* *OR* **compile** using: `make THREADS=XX FILE=YY`, where `THREADS=XX` specifies the number of threads to be used in the parallel section of the code.

* To **compile and run**, use: `make THREADS=XX FILE=YY test`, where `FILE=YY` is the graph file to execute right after the target is compiled.

* **Run**: `./ex10 <graph filepath> [# of terminals] [-s]`. `[# of terminals]` is the number of starting terminals to check through to obtain best objective value. Include `[-s]` flag to print edges of Steiner-subgraph.

***

* `make check` runs the cppcheck static analysis check.

* `make coverage` runs gcov coverage test with lcov results output in folder result/.

* `make doc` compiles doxygen documentation in folder html/.

* `make cleancoverage` cleans all files related to coverage test, except the results folder.
 
* `make clean` cleans all files created by the Makefile, including the coverage-test related files.