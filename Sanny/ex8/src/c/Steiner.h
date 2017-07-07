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

using EdgesWithWeight = std::pair<std::vector<Edge >, int >;
using Primes = std::vector<int >;
using Nodes = std::vector<int >;

class Steiner {
private:
	Dijkstra* dijsktra;
	Primes getPrimes(unsigned int vertexCount);
	void addToSteiner(Edge edge, unsigned int i,
			EdgesWithWeight* edgesAndWeight, Weights& weights);

public:
	Steiner();
	virtual ~Steiner();
	EdgesWithWeight steiner(int vertexCount, Edges& edges, Weights& weights, int startnode);
};

#endif /* C_STEINER_H_ */
