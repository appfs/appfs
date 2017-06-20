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

class Dijkstra {
public:
	Dijkstra();
	virtual ~Dijkstra();
	std::vector<int> dijkstra(unsigned int vertexCount, std::vector<std::pair<int, int>>& edges, std::vector<int>& weights, unsigned int source);
};

#endif /* C_DIJKSTRA_H_ */
