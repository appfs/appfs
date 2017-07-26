/*
 * DijkstraSolver.cpp
 *
 * 	\brief     My dijkstra implementation for the tenth exercise
 *  \details   This class computes the longest shortest path for given startingPoint
 *  \author    Julia Baumbach
 *  \date      15.07.2017
 */


#ifndef DIJKSTRASOLVER_H_
#define DIJKSTRASOLVER_H_

#include <vector>
#include "GraphReader.h"

/*
 * \typedef VisitedMap which stores at i-th position if vertex i is already visited
 */
using VisitedMap = std::vector<bool>;

class DijkstraSolver {
public:
	DijkstraSolver(SortedEdges, unsigned int);

	void computeShortestPath(unsigned int, WeightMap&, std::vector<int>&);
	int setEdgeWeightToZero(int, int);

private:
	SortedEdges sortedEdges;
	unsigned int numberOfVertices;
};

#endif /* DIJKSTRASOLVER_H_ */
