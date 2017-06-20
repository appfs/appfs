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
class prioritize{public: bool operator ()(std::pair<int, int>&p1 ,std::pair<int, int>&p2){return p1.second>p2.second;}};
typedef std::priority_queue<std::pair<int, int>,std::vector<std::pair<int, int>>, prioritize > queue;

std::vector<int> Dijkstra::dijkstra(unsigned int vertexCount, std::vector<std::pair<int, int>>& edges, std::vector<int>& weights, unsigned int source){
	std::vector<int> weightMap(vertexCount);
	weightMap[source] = 0;
	for(unsigned int i = 0; i<=vertexCount; i++){
		if(source == i){
			continue;
		}
		weightMap[i] = INT_MAX;
	}

	queue Q;
	Q.push(std::pair<int, int>(source, 0));

	while(!Q.empty()){
		std::pair<int, int> top = Q.top();
		Q.pop();
		int vertex = top.first;
		int dist = top.second;
		if(dist > weightMap[vertex]){
			continue;
		}
		for(unsigned int i = 0; i < edges.size(); i++){
			std::pair<int, int> edge = edges[i];
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
			Q.push(std::pair<int, int>(targetVertex, weightMap[targetVertex]));
		}

	}
	return weightMap;
}

