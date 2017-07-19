/*
 * Dijkstra.cpp
 *
 *  Created on: 20.06.2017
 *      Author: alesan
 */

#include "Dijkstra.h"

Dijkstra::Dijkstra(int vertexCount, NodeToEdgeMap* vertexToEdges, Weights* weights) {
	this->vertexCount = vertexCount;
	this->vertexToEdges = vertexToEdges;
	this->weights = weights;

}

Dijkstra::~Dijkstra() {
	// nop
}

/** Computes the shortest path with the Dijkstra-Algo in O(log(m)n) */
WeightsAndPrenodeMap Dijkstra::computeShortestPath(int source){
	WeightsAndPrenodeMap weightAndNodeMap(vertexCount);
	weightAndNodeMap[source].first = 0.;

	#pragma omp parallel for
	for(int i = 0; i<vertexCount; i++){
		if(source == i){
			continue;
		}
		weightAndNodeMap[i].first = INT_MAX;
		weightAndNodeMap[i].second = -1;
	}

	Queue* Q = new Queue();
	Q->push(Edge(source, 0));

	while(!Q->empty()){
		Edge top = Q->top();
		Q->pop();
		int vertex = top.first;
		double dist = top.second;
		if(dist > weightAndNodeMap[vertex].first){
			continue;
		}
		
		EdgeWithIndexList edgeList = vertexToEdges->at(vertex);
		for(unsigned int i = 0; i < edgeList.size(); i++){
			EdgeWithIndex edgeWithIndex = edgeList[i];
			if(weightAndNodeMap[edgeWithIndex.first].first <= weightAndNodeMap[vertex].first + weights->at(edgeWithIndex.second)){
				continue;
			}
			weightAndNodeMap[edgeWithIndex.first].second = vertex;
			weightAndNodeMap[edgeWithIndex.first].first = weightAndNodeMap[vertex].first + weights->at(edgeWithIndex.second);
			Q->push(Edge(edgeWithIndex.first, weightAndNodeMap[edgeWithIndex.first].first));
		}

	}
	delete Q;
	return weightAndNodeMap;
}

