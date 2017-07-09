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

Steiner::Steiner(int numberOfVertices, Edges edges, WeightMap weights):
	numberOfVertices(numberOfVertices), edges(edges), weights(weights){
	terminals = computePrimes();
	for(int prime : terminals){
		std::cout << "terminal " << prime << std::endl;
	}
}

Edges Steiner::solve(int startNode){
	Edges result;
	VisitedMap alreadyAdded(numberOfVertices);
	for(int i = 0; i <=numberOfVertices; i++){
		alreadyAdded[i] = false;
	}
	dijkstra myDijkstra(weights, edges, numberOfVertices);
	int counter = 0;
	WeightMap weightMap(numberOfVertices);
	std::vector<int> predecessorMap(numberOfVertices);

	while(counter <= terminals.size()){
		myDijkstra.computeShortestPath(startNode, weightMap, predecessorMap);
		for(int weight : weightMap){
			std::cout << "weight for round" << counter << " " << weight << std::endl;
		}
		int vertexToAdd = findNearestTerminal(weightMap);
		std::cout << "nearest terminal " << vertexToAdd << std::endl;
		while(vertexToAdd != startNode){
			if(alreadyAdded[vertexToAdd]){
				break;
			}
			result.push_back(std::make_pair(vertexToAdd, predecessorMap[vertexToAdd]));
			myDijkstra.updateEdgeWeight(vertexToAdd, predecessorMap[vertexToAdd]);
			myDijkstra.updateEdgeWeight(predecessorMap[vertexToAdd], vertexToAdd);
			alreadyAdded[vertexToAdd] = true;
			vertexToAdd = predecessorMap[vertexToAdd];
		}
		counter++;
	}


	return result;
}

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


bool Steiner::hasDivider(const std::vector<int>& result, int i) {
	for (int prime : result) {
		if ((i % prime) == 0 || prime/2 >= i) {
			return true;
		}
	}return false;
}

std::vector<int> Steiner::computePrimes(){
	std::vector<int> result;
	result.push_back(2);
	for(int i = 3; i<numberOfVertices; i++){
		if(!hasDivider(result, i)){
			result.push_back(i);
		}
	}
	return result;
}


