GPHDIR := ../../SP
N_NODES =  1 2 3 4 5 6 7 8 9 10 11

ex10: ex10.cpp
	g++ -I /usr/local/boost_dir ex10.cpp -o ex10 -std=c++11 -fopenmp
	

test:
	OMP_NUM_THREADS=$(THREADS) ./ex10 $(GPHDIR)/$(FILE) $(N_NODES)