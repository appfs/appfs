makefile provides the following commands:

make: compile
make doc: create doxygen documentation
make coverage: create coverage report in dir coverage
make check: run cppcheck
make test FILE=X THREADS=Y: run program for file X in dir ../../SP with Y threads and 100 starting points

otherwise the program can be run with: ./ex10 <graph.gph> <n_start_pts> (other options)
type ./ex10 -h for more detail
