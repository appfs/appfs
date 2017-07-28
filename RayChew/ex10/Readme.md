# Exercise 10
Calculate the best Steiner Tree for a given number of starting terminals (default is 100) and a given graph file.

* **Compile**: `make THREADS=XX`, where `THREADS=XX` specifies the number of threads to be used in the parallel section of the code.
* **compile and run**: `make THREADS=XX FILE=YY test`, where `FILE=YY` is the graph file to execute right after the target is compiled.
* **Run**: `./ex10 <graph filepath> [# of terminals] [-s]`. `[# of terminals]` is the number of starting terminals to check through to obtain best objective value. Include `[-s]` flag to print edges of Steiner-subgraph.

***

* `make check` runs the cppcheck static analysis check.
* `make coverage` runs gcov coverage test with lcov results output in folder result/.
* `make doc` compiles doxygen documentation in folder html/.
* `make cleancoverage` cleans all files related to coverage test, except the results folder.
* `make clean` cleans all files created by the Makefile, including the coverage-test related files.

***

**To compile without Makefile**: 
  1. Compile utils.cxx: `g++ -std=c++14 -O3 -march=native -pedantic -Wall -c src/utils.cxx -o utils.o -lboost_timer -lboost_program_options -lboost_system -fopenmp`
  2. Compile heap.cxx: `g++ -std=c++14 -O3 -march=native -pedantic -Wall -c src/heap.cxx -o heap.o -lboost_timer -lboost_program_options -lboost_system -fopenmp`
  3. Compile steiner.cxx: `g++ -std=c++14 -O3 -march=native -pedantic -Wall -c src/steiner.cxx -o steiner.o -lboost_timer -lboost_program_options -lboost_system -fopenmp`
  4. Compile checker.cxx: `g++ -std=c++14 -O3 -march=native -pedantic -Wall -c src/checker.cxx -o checker.o -lboost_timer -lboost_program_options -lboost_system -fopenmp`
  5. Link and output: `g++ -std=c++14 -O3 -march=native -pedantic -Wall src/utils.o src/heap.o src/steiner.o src/checker.o src/ex10.cxx -o ex10 -lboost_timer -lboost_program_options -lboost_system [-fopenmp]`. Include  `-fopenmp` to enable parallelization.