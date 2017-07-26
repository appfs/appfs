GPHDIR := ../../SP
N_NODES =  100

ex10: ex10.cpp
	g++ -I /usr/local/boost_dir ex10.cpp -o ex10 -std=c++11 -fopenmp -march=native
	
test:
	OMP_NUM_THREADS=$(THREADS) ./ex10 $(GPHDIR)/$(FILE) $(N_NODES)