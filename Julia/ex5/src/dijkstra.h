/*
 * dijkstra.h
 *
 *  Created on: Jun 19, 2017
 *      Author: july
 */

#ifndef DIJKSTRA_H_
#define DIJKSTRA_H_

#include <vector>

/*
 * \typedef Edges which representes all edges in a graph in a list
 */
using Edges = std::vector<std::pair<int, int> >;
using Edge = std::pair<int, int>;
/*
 * \typedef WeightMap which representes the weight of the i-th edge as the i-th position
 */
using WeightMap = std::vector<int>;
/*
 *
 */
using DijkstraPair = std::pair<int, int>;
/*
 * \typedef SortedEdges which safes the all edges with weights belonging to one vertex in a (index, weight) pair
 */
using SortedEdges = std::vector<std::vector<DijkstraPair> >;

class dijkstra {
public:
	dijkstra(WeightMap, Edges, unsigned int);

	WeightMap computeShortestPath();

private:
	SortedEdges sortedEdges;
	unsigned int numberOfVertices;
};

#endif /* DIJKSTRA_H_ */
