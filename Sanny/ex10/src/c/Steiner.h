/*
 * Steiner.h
 *
 *  Created on: 06.07.2017
 *      Author: alesan
 */

#ifndef C_STEINER_H_
#define C_STEINER_H_

#include <algorithm>
#include "Dijkstra.h"

using Primes = std::vector<int >;
using Nodes = std::vector<int >;

class Steiner {
private:
	int steinerWeight = 0;
	Nodes* nodesInTree;
	Edges* steinerEdges;
	Primes getPrimes(unsigned int vertexCount);
	void addToSteiner(Edge edge, unsigned int i, Weights* weights);

public:
	Steiner();
	virtual ~Steiner();
	void steiner(int vertexCount, Edges* edges, Weights& weights, int startnode);
	int getWeight();
	Edges getEdges();
	Nodes getNodes();
};

#endif /* C_STEINER_H_ */
