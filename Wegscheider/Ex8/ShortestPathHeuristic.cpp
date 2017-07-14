/**
 * @file 	ShortestPathHeuristic.cpp
 * @author 	Fabian Wegscheider
 * @date 	Jul 10, 2017
 */

#include <boost/heap/fibonacci_heap.hpp>
#include "MyDijkstra.h"
#include "ShortestPathHeuristic.h"



using Pair = std::pair<int, double>;
using std::vector;

/**
 * Data that is stored in one node of a heap. Contains an integer and a double.
 * Comparisons are made by the double, smaller has higher priority
 */
struct heap_data
{
    heap::fibonacci_heap<heap_data>::handle_type handle;
    Pair pair;

    heap_data(Pair p):
        pair(p)
    {}

    bool operator<(heap_data const & rhs) const {
        return pair.second > rhs.pair.second;
    }
};


/*
 * Finds all primes in {2,...,n} using the fact that if a natural number
 * has a divisor it also has at least one prime divisor. the resulting vector
 * contains the numbers -1.
 */
vector<int> ShortestPathHeuristic::findPrimes(int n) {
	assert(n >= 2);

	vector<int> primes;
	primes.push_back(2);

	for (int i = 3; i < n; ++i) {
		bool isPrime = true;
		for (unsigned int j = 0; j < primes.size() && primes[j]*primes[j] <= i; j++) {
			if (i % primes[j] == 0) {
				isPrime = false;
				break;
			}
		}
		if (isPrime) {
			primes.push_back(i);
		}
	}

	for (unsigned int i = 0; i < primes.size(); ++i) {
		primes[i]--;
	}

	return primes;
}



double ShortestPathHeuristic::constructSteinerTree(Graph& g, int numVertices, int source) {
	assert(numVertices >= 2);

	using Heap = heap::fibonacci_heap<heap_data>;

	vector<int> primes = findPrimes(numVertices);
	int numTerminals = primes.size();

	double** distances = new double*[numTerminals];
	int** predecessors = new int*[numTerminals];

	/*shortest paths from all terminals to all other nodes are calculated*/
	for (int i = 0; i < numTerminals; ++i) {
		if (primes[i] != source) {
			distances[i] = new double[numVertices];
			predecessors[i] = new int[numVertices];
			MyDijkstra::computeShortestPaths(g, numVertices, primes[i], distances[i], predecessors[i]);
		} else {
			distances[i] = new double[0];
			predecessors[i] = new int[0];
		}
	}

	/*we use a priority queue to keep track of remaining and closest terminals*/
	Heap heap;
	Heap::handle_type *handles = new Heap::handle_type[numTerminals];

	//all terminals except for the source are added to heap
	for (int i = 0; i < numTerminals; ++i) {
		if (primes[i] != source) {
			handles[i] = heap.push(std::make_pair(i, distances[i][source]));
		}
	}

	//we also keep track of the closest node in the tree for each terminal
	double objectiveValue = 0;
	int nearestNodes[numTerminals];
	for (int i = 0; i < numTerminals; i++) {
		nearestNodes[i] = source;
	}

	while (!heap.empty()) {
		Pair nextTerminal = heap.top().pair;
		int connectionNode = nearestNodes[nextTerminal.first];
		heap.pop();			//top element in heap always is the closest to tree

		objectiveValue += (distances[nextTerminal.first])[connectionNode];

		//now we iterate over all remaining terminals to check whether
		//they have become closer to the new tree
		for (Heap::iterator it = heap.begin(); it != heap.end(); ++it) {
			Pair curr = (*it).pair;
			double min = curr.second;
			int pred = connectionNode;
			bool reachedEnd = false;

			//for each terminal we iterate over all vertices on newly added path
			while (!reachedEnd){
				reachedEnd = (pred == primes[nextTerminal.first]);
				double tmp = (distances[curr.first])[pred];
				if (tmp < min) {
					min = tmp;
					nearestNodes[curr.first] = pred;
				}
				pred = predecessors[nextTerminal.first][pred];
			}

			//update of heap if neccessary
			if (min < curr.second) {
				(*handles[curr.first]).pair.second = min;
				heap.increase(handles[curr.first]);
			}
		} //end of iteration through heap

	} //end of algorithm


	for (int i = 0; i < numTerminals; i++) {
		delete[] distances[i];
		delete[] predecessors[i];
	}
	delete[] distances;
	delete[] predecessors;
	delete[] handles;

	return objectiveValue;


}

double ShortestPathHeuristic::constructSteinerTree(Graph& g, int numVertices) {
	return constructSteinerTree(g, numVertices, 1);
}


