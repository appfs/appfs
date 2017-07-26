/*
 * Steiner.h
 *
 * 	\brief     My steiner problem solver for ex8
 *  \details   This class solves a steiner tree problem
 *  \author    Julia Baumbach
 *  \date      02.07.2017
 */

#include "Steiner.h"
#include <iostream>
#include <climits>
#include <algorithm>

/**
 * \fn Constructor
 * \brief Construct a Steiner instance by initializing numberOfVertices, edges and weights. Computes also the terminals of the graph.
 */
Steiner::Steiner(int numberOfVertices, Edges edges, WeightMap weights, std::vector<int> terminals):
	numberOfVertices(numberOfVertices), edges(edges), weights(weights), terminals(terminals){
	objectiveValue = 0;
}

/**
 * \fn Edges solve(int startNode)
 * \brief Solves the Steiner problem for a given start node
 * \return Edges of the minimal spanning tree
 */
Edges Steiner::solveSteiner(unsigned int startNode){
	Edges result;
	VisitedMap alreadyAdded(numberOfVertices, false);

	dijkstra myDijkstra(weights, edges, numberOfVertices);
	WeightMap weightMap(numberOfVertices, INT_MAX);
	std::vector<int> predecessorMap(numberOfVertices, -1);

	while(!terminals.empty()){
		myDijkstra.computeShortestPath(startNode, weightMap, predecessorMap);

		int vertexToAdd = findNearestTerminal(weightMap);
		//Remove nearest terminal from terminal list
		terminals.erase(std::find(terminals.begin(), terminals.end(), vertexToAdd));
		while(vertexToAdd != startNode){
			//break if vertexToAdd is already in the edgeList. Means, that also all predecessors are in the list.
			if(alreadyAdded[vertexToAdd]){
				break;
			}

			result.push_back(std::make_pair(vertexToAdd, predecessorMap[vertexToAdd]));
			//Update both edge-weights, for edge vertexToAdd-Predecessor and vice-versa
			objectiveValue += myDijkstra.setEdgeWeightToZero(vertexToAdd, predecessorMap[vertexToAdd]);
			myDijkstra.setEdgeWeightToZero(predecessorMap[vertexToAdd], vertexToAdd);

			alreadyAdded[vertexToAdd] = true;
			vertexToAdd = predecessorMap[vertexToAdd];
		}
	}
	return result;
}

int Steiner::getObjectiveValue(){
	return objectiveValue;
}

/*
 * \fn int findNearestTerminal(WeightMap)
 * \brief finds the lowest weight of a terminal in a weight-map
 * \return index of the nearest terminal
 */
int Steiner::findNearestTerminal(WeightMap weightMap){
	int nearestWeight = INT_MAX;
	int nearestVertex = 0;
	for(int prime : terminals){
		if(weightMap[prime] < nearestWeight){
			nearestWeight = weightMap[prime];
			nearestVertex = prime;
		}
	}
	return nearestVertex;
}


