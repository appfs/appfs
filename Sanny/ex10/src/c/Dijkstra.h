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

using Edge = std::pair<int, int >;
using Edges = std::vector<Edge >;
using Weights = std::vector<double >;
using WeightsAndPrenodeMap = std::vector<std::pair<double, int >>;
using EdgeWithIndex = std::pair<int, double >;
using EdgeWithIndexList = std::vector<EdgeWithIndex >;
using NodeToEdgeMap = std::vector<EdgeWithIndexList >;

class prioritize{
	public: bool operator ()(Edge&p1 ,Edge&p2){
		return p1.second>p2.second;
	}
};
using Queue = std::priority_queue<Edge, Edges, prioritize >;

class Dijkstra {
private:
	int vertexCount;
	NodeToEdgeMap* vertexToEdges;
	Weights* weights;

public:
	Dijkstra(int vertexCount, NodeToEdgeMap* vertexToEdges, Weights* weights);
	virtual ~Dijkstra();
	WeightsAndPrenodeMap computeShortestPath(int source);
};

#endif /* C_DIJKSTRA_H_ */
