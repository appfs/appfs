# Exercise 10
Calculate the best Steiner Tree for a given number of starting terminals (default is 100) and a given graph file.

* Compile: `g++ -std=c++14 -O3 -march=native -pedantic -Wall src/utils.o src/heap.o src/steiner.o src/checker.o src/ex10.cxx -o ex10 -lboost_timer -lboost_program_options -lboost_system [-fopenmp]`. Include  `-fopenmp` to enable parallelization.

* *OR* compile using: `make THREADS=XX FILE=YY`, where `THREADS=XX` specifies the number of threads to be used in the parallel section of the code and `FILE=YY` is the graph file to execute right after the target is compiled.

* Run: `./ex10 <graph filepath> [# of terminals] [-s]`. `[# of terminals]` is the number of starting terminals to check through to obtain best objective value. Include `[-s]` flag to print edges of Steiner-subgraph.