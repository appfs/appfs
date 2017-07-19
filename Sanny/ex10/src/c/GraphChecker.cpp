/*
 * GraphChecker.cpp
 *
 *  Created on: 08.07.2017
 *      Author: alesan
 */

#include "GraphChecker.h"

GraphChecker::GraphChecker(Edges edges, Nodes nodes, Nodes terminals) {
	this->edges = edges;
	this->nodes = nodes;
	this->terminals = terminals;
}

GraphChecker::~GraphChecker() {
	//nop
}

/** Search and collects all adjacents for each node*/
AdjacentsMap GraphChecker::makeAdjacentsMap() {
	AdjacentsMap adjacentsMap = AdjacentsMap();
	for (unsigned int i = 0; i < nodes.size(); i++) {
		Adjacents* adjs = new Adjacents();
		getAdjacents(nodes[i], adjs);
		adjacentsMap.push_back(*adjs);
		delete adjs;
	}
	return adjacentsMap;
}

/** gets all adjacents and returns them in the given param adjs*/
void GraphChecker::getAdjacents(int node, Adjacents* adjs) {
	// collect adjacents
	for (unsigned int i = 0; i < edges.size(); i++) {
		if (edges[i].first != node && edges[i].second != node) {
			continue;
		}
		int endNode = -1;
		if (edges[i].first == node) {
			endNode = edges[i].second;
		} else {
			endNode = edges[i].first;
		}
		for (unsigned int j = 0; j < nodes.size(); j++) {
			if (nodes[j] == endNode) {
				adjs->push_back(std::make_pair(j, endNode));
			}
		}
	}
}

/** Check wether the graph is connected or not via BFS */
bool GraphChecker::isConnected(){
	AdjacentsMap adjacentsMap = makeAdjacentsMap();

	char *visited = new char[nodes.size()];
	#pragma omp parallel for
	for (unsigned int i = 0; i < nodes.size(); i++){
		visited[i] = 0;
	}
	NodesToVisit* nodesToVisit = new NodesToVisit();
	nodesToVisit->push(0);
	visited[0] = 1;

	while(!nodesToVisit->empty()){
		int nodeIndex = nodesToVisit->front();
		nodesToVisit->pop();
		Adjacents adjs = adjacentsMap[nodeIndex];
		for(unsigned int i = 0; i < adjs.size(); i++){
			Adjacent adj = adjs[i];
			if(visited[adj.first] == 0){
				nodesToVisit->push(adj.first);
				visited[adj.first] = 1;
			}
		}

	}
	for (unsigned int i = 0; i < nodes.size(); i++){
		if(visited[i] == 0){
			delete [] visited;
			return false;
		}
	}
	delete [] visited;
	delete nodesToVisit;
	return true;
}

/** Checks recursive if the graph has a cycle via DFS */
bool GraphChecker::hasCycle(){
    char* visited = new char[nodes.size()];

	AdjacentsMap adjacentsMap = makeAdjacentsMap();
	#pragma omp parallel for
    for (unsigned int i = 0; i < nodes.size(); i++){
        visited[i] = 0;
    }
    for(unsigned int i = 0; i < nodes.size(); i++){
    	if(visited[i] == 0){
    		if (hasCycle(i, visited, -1, adjacentsMap)){
    			delete [] visited;
    			return true;
    		}
    	}
    }
    delete [] visited;
	return false;
}

/** Checks recursive if the graph has a cycle via DFS */
bool GraphChecker::containsAllTerminals(){
    char* contained = new char[terminals.size()];

	#pragma omp parallel for
    for (unsigned int i = 0; i < terminals.size(); i++){
    	contained[i] = 0;
    }

    for (unsigned int j = 0; j < terminals.size(); j++){
    	int terminal = terminals[j];
    	for(unsigned int i = 0; i < nodes.size(); i++){
        	if(terminal == nodes[i]){
        		contained[j] = 1;
        	}
        }
    }

    for (unsigned int i = 0; i < terminals.size(); i++){
    	if(contained[i] == 0){
    		return false;
    	}
    }
    delete [] contained;
	return true;
}

bool GraphChecker::hasCycle(int nodeIndex, char* visited, int parentIndex, AdjacentsMap adjacentsMap){
	visited[nodeIndex] = 1;
	Adjacents adjs = adjacentsMap[nodeIndex];

    for (unsigned int i = 0; i < adjs.size(); i++){
    	Adjacent adj = adjs[i];
        if (!visited[adj.first]){
           if (hasCycle(adj.first, visited, nodeIndex, adjacentsMap) == 1){
        	   return true;
           }
        }

        else if (adj.first != parentIndex){
        	return true;
        }
    }
	return false;
}

