# Exercise 10
Calculate the best Steiner Tree for a given number of starting terminals (default is 100) and a given graph file.
* Compile: `g++ -std=c++14 -O3 -march=native -pedantic -Wall src/utils.o src/heap.o src/steiner.o src/checker.o src/ex10.cxx -o ex10 -lboost_timer -lboost_program_options -lboost_system (-fopenmp)`

Include  `-fopenmp` to enable parallelization.

* *OR* compile using: `make THREADS=XX FILE=YY`,

where `THREADS=XX` specifies the number of threads to be used in the parallel section of the code.

`FILE=YY` is the graph file to use right after the target is compiled.

* Run: `./ex10 <graph filepath> [# of terminals] [-s]`.

`[# of terminals]` is the number of starting terminals to check through to obtain best objective value.

Include `[-s]` flag to print edges of Steiner-subgraph.

## Results
All results were obtained with 1 thread used.

***
b15.gph

There are 25 terminals.

Solution is a tree.

Solution is feasible.


TLEN: 214

TIME: 0s

WALL: 0.0002344s

***
i640-345.gph

There are 115 terminals.

Solution is a tree.

Solution is feasible.


TLEN: 14156

TIME: 0.01s

WALL: 0.006286s

***
es10kaz.gph

There are 5442 terminals.

Solution is a tree.

Solution is feasible.


TLEN: 489217756

TIME: 0.06s

WALL: 0.06315s

***
world666.gph

There are 121 terminals.

Solution is a tree.

Solution is feasible.


TLEN: 117613

TIME: 0.02s

WALL: 0.01729s

***
big8.gph

There are 5133 terminals.

Solution is a tree.

Solution is feasible.


TLEN: 672340

TIME: 0.13s

WALL: 0.1363s

***
acycpos-1.gph

There are 12251 terminals.

Solution is a tree.

Solution is feasible.


TLEN: 97850

TIME: 0.55s

WALL: 0.5751s

***
phard-x.gph

There are 155611 terminals.

Solution is a tree.

Solution is feasible.


TLEN: 18665398

TIME: 2.6s

WALL: 2.787s

***
ssquare-1.gph

There are 82025 terminals.

Solution is a tree.

Solution is feasible.


TLEN: 689277609

TIME: 0.97s

WALL: 1.03s

***
rand14-5.gph

There are 564 terminals.

Solution is a tree.

Solution is feasible.


TLEN: 1336

TIME: 0.21s

WALL: 0.2595s

***
rand4-1.gph

There are 82025 terminals.

Solution is a tree.

Solution is feasible.


TLEN: 899483

TIME: 2.22s

WALL: 2.297s

***
ssquare-s-5.gph

There are 82025 terminals.

Solution is a tree.

Solution is feasible.


TLEN: 687822485

TIME: 1.07s

WALL: 1.127s

***
phard-x.gph

There are 155611 terminals.

Solution is a tree.

Solution is feasible.


TLEN: 18665398

TIME: 2.59s

WALL: 2.796s

***
acycpos-x.gph

There are 53228 terminals.

Solution is a tree.

Solution is feasible.


TLEN: 411524

TIME: 4.86s

WALL: 5.209s

***
rand4-x.gph

There are 3512 terminals.

Solution is a tree.

Solution is feasible.


TLEN: 8815

TIME: 3.59s

WALL: 3.965s

***
ssquare-x.gph

There are 1077871 terminals.

Solution is a tree.

Solution is feasible.


TLEN: 23092454892

TIME: 28.9s

WALL: 41.6s

***
ssquare-s-x.gph

There are 1077871 terminals.

Solution is a tree.

Solution is feasible.


TLEN: 23071754551

TIME: 27.72s

WALL: 72.03s