/*
 * Dijkstra.h
 *
 *  Created on: 20.06.2017
 *      Author: alesan
 */

#ifndef C_DIJKSTRA_H_
#define C_DIJKSTRA_H_

#include <vector>
#include <climits>
#include <queue>

using Edge = std::pair<int, int>;
using Edges = std::vector<Edge >;
using Weights = std::vector<int >;

class Dijkstra {
public:
	Dijkstra();
	virtual ~Dijkstra();
	Weights dijkstra(unsigned int vertexCount, Edges& edges, Weights& weights, unsigned int source);
};

#endif /* C_DIJKSTRA_H_ */
