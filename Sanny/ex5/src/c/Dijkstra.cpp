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

class prioritize{
	public: bool operator ()(Edge&p1 ,Edge&p2){
		return p1.second>p2.second;
	}
};

typedef std::priority_queue<Edge, Edges, prioritize > queue;

Weights Dijkstra::dijkstra(unsigned int vertexCount, Edges& edges, Weights& weights, unsigned int source){
	Weights weightMap(vertexCount);
	weightMap[source] = 0;
	for(unsigned int i = 0; i<=vertexCount; i++){
		if(source == i){
			continue;
		}
		weightMap[i] = INT_MAX;
	}

	queue Q;
	Q.push(Edge(source, 0));

	while(!Q.empty()){
		Edge top = Q.top();
		Q.pop();
		int vertex = top.first;
		int dist = top.second;
		if(dist > weightMap[vertex]){
			continue;
		}
		for(unsigned int i = 0; i < edges.size(); i++){
			Edge edge = edges[i];
			int targetVertex;

			if(edge.first == vertex){
				targetVertex = edge.second;
			} else if(edge.second == vertex){
				targetVertex = edge.first;
			} else {
				continue;
			}

			int weight = weights[i];
			if(weightMap[targetVertex] <= weightMap[vertex] + weight){
				continue;
			}

			weightMap[targetVertex] = weightMap[vertex] + weight;
			Q.push(Edge(targetVertex, weightMap[targetVertex]));
		}

	}
	return weightMap;
}

