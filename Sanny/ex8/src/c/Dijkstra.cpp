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

WeightsAndPrenodeMap Dijkstra::dijkstra(int vertexCount, Edges& edges, Weights& weights, int source){
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
		if(dist > weightAndNodeMap[vertex].first){
			continue;
		}
		vertexToEdges[vertex];
		
		for(unsigned int i = 0; i < vertexToEdges[vertex].size(); i++){
			EdgeWithWeight edgeWithWeight = vertexToEdges[vertex][i];
			if(weightAndNodeMap[edgeWithWeight.first].first <= weightAndNodeMap[vertex].first + edgeWithWeight.second){
				continue;
			}
			weightAndNodeMap[edgeWithWeight.first].second = vertex;
			weightAndNodeMap[edgeWithWeight.first].first = weightAndNodeMap[vertex].first + edgeWithWeight.second;
			Q.push(Edge(edgeWithWeight.first, weightAndNodeMap[edgeWithWeight.first].first));
		}

	}
	return weightAndNodeMap;
}

