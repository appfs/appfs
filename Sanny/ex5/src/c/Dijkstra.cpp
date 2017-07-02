/*
 * Dijkstra.cpp
 *
 *  Created on: 20.06.2017
 *      Author: alesan
 */

#include "Dijkstra.h"

Dijkstra::Dijkstra() {
	// nop

}

Dijkstra::~Dijkstra() {
	// nop
}

Weights Dijkstra::dijkstra(unsigned int vertexCount, Edges& edges, Weights& weights, unsigned int source){
	Weights weightMap(vertexCount);
	weightMap[source] = 0.;
	for(unsigned int i = 0; i<vertexCount; i++){
		if(source == i){
			continue;
		}
		weightMap[i] = INT_MAX;
	}

	NodeToEdgeMap vertexToEdges(vertexCount);
	for(unsigned int i = 0; i < edges.size(); i++){
		Edge edge = edges[i];
		vertexToEdges[edge.first].push_back(std::make_pair(edge.second, weights[i]));
		vertexToEdges[edge.second].push_back(std::make_pair(edge.first, weights[i]));
	}

	Queue Q;
	Q.push(Edge(source, 0));

	while(!Q.empty()){
		Edge top = Q.top();
		Q.pop();
		int vertex = top.first;
		double dist = top.second;
		if(dist > weightMap[vertex]){
			continue;
		}
		vertexToEdges[vertex];
		for(EdgeWithWeight edgeWithWeight : vertexToEdges[vertex]){
			if(weightMap[edgeWithWeight.first] <= weightMap[vertex] + edgeWithWeight.second){
				continue;
			}

			weightMap[edgeWithWeight.first] = weightMap[vertex] + edgeWithWeight.second;
			Q.push(Edge(edgeWithWeight.first, weightMap[edgeWithWeight.first]));
		}

	}
	return weightMap;
}

