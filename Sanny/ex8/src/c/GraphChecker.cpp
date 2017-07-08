/*
 * GraphChecker.cpp
 *
 *  Created on: 08.07.2017
 *      Author: alesan
 */

#include "GraphChecker.h"

GraphChecker::GraphChecker(Edges edges, Nodes nodes) {
	this->edges = edges;
	this->nodes = nodes;
}

GraphChecker::~GraphChecker() {
	// nop
}

char GraphChecker::isConnected(){

	return 1;
}

char GraphChecker::hasCycle(){
    char *visited = new char[nodes.size()];
    for (unsigned int i = 0; i < nodes.size(); i++){
        visited[i] = 0;
    }
    for(unsigned int i = 0; i < nodes.size(); i++){
    	if(visited[i] == 0){
    		if (hasCycle(i, visited, -1) == 1)
    			return 1;
    	}
    }
	return 0;
}
char GraphChecker::hasCycle(int nodeIndex, char* visited, int parentIndex){
	visited[nodeIndex] = 1;
	int node = nodes[nodeIndex];
	Adjacents adjs = Adjacents();
	// collect adjacents
	for (unsigned int i = 0; i < edges.size(); i++){
		if (edges[i].first != node && edges[i].second != node){
			continue;
		}
		int endNode = -1;
		if (edges[i].first == node){
			endNode = edges[i].second;
		} else {
			endNode = edges[i].first;
		}
		for (unsigned int j = 0; j < nodes.size(); j++){
			if(nodes[j] == endNode){
				adjs.push_back(std::make_pair(j, endNode));
			}
		}
	}

    for (unsigned int i = 0; i < adjs.size(); i++){
    	Adjacent adj = adjs[i];
        if (!visited[adj.first]){
           if (hasCycle(adj.first, visited, nodeIndex) == 1){
              return 1;
           }
        }

        else if (adj.first != parentIndex){
           return 1;
        }
    }
	return 0;
}

