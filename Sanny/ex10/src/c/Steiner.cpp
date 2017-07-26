/*
 * Steiner.cpp
 *
 *  Created on: 06.07.2017
 *      Author: alesan
 */

#include "Steiner.h"

/** Constructor */
Steiner::Steiner() {
	nodesInTree = new Nodes();
	steinerEdges = new Edges();
}

/** Deconstructor */
Steiner::~Steiner() {
	delete nodesInTree;
	delete steinerEdges;
}

/** Getter for Edges in Steiner-tree */
Edges Steiner::getEdges() {
	return *steinerEdges;
}

/** Getter for Edges in Steiner-tree */
int Steiner::getWeight() {
	return steinerWeight;
}

/** Getter for Nodes in Steiner-tree */
Nodes Steiner::getNodes() {
	return *nodesInTree;
}

void Steiner::addToSteiner(Edge edge, unsigned int i, Weights* localWeights) {
	steinerEdges->push_back(edge);
	steinerWeight += localWeights->at(i);
	localWeights->at(i) = 0;
}

/** Solves the Steiner-Tree by computing the shortest Path via Dijkstra and searching for the nearest terminal untill no terminal is left */
void Steiner::computeSteinerTree(int vertexCount, Edges* edges, Weights& weights, Terminals& terminals, int startnode) {
	NodeToEdgeMap* vertexToEdges = new NodeToEdgeMap(vertexCount);
	for(unsigned int i = 0; i < edges->size(); i++){
		Edge edge = edges->at(i);
		vertexToEdges->at(edge.first).push_back(std::make_pair(edge.second, i));
		vertexToEdges->at(edge.second).push_back(std::make_pair(edge.first, i));
	}
	Weights* localWeights = new Weights(weights);

	Dijkstra* dijsktra = new Dijkstra(vertexCount, vertexToEdges, localWeights);
	steinerEdges->clear();
	nodesInTree->clear();

	if(vertexCount < startnode){
		return;
	}

	Terminals* localTerminals = new Terminals(terminals);
	if(std::find(localTerminals->begin(), localTerminals->end(), startnode) != localTerminals->end()){
		localTerminals->erase(std::find(localTerminals->begin(), localTerminals->end(), startnode));
	}
	nodesInTree->push_back(startnode);

	// while there aren't all primes in the steiner-tree
	while(!localTerminals->empty()){
		//compute distances
		Weights w = *localWeights;
		WeightsAndPrenodeMap map = dijsktra->computeShortestPath(startnode);
		int primeToAdd = 0;
		int distToPrime = INT_MAX;
		//go through all missing primes and find the nearest
		for (unsigned int i = 0; i<localTerminals->size(); i++){
			int prime = localTerminals->at(i);
			if(map[prime].first < distToPrime){
				primeToAdd = prime;
				distToPrime = map[prime].first;
			}
		}
		// construct the path to steiner tree
		int node = primeToAdd;
		int preNode = map[node].second;
		do{
			// are we in the steiner part?
			if(std::find(nodesInTree->begin(), nodesInTree->end(), node) != nodesInTree->end()){
				break;
			}
			// add node and edge to steiner-tree and set edgeweight to 0 for included steinerEdges
			nodesInTree->push_back(node);
			EdgeWithIndexList edgesFromNode = vertexToEdges->at(node);
			for (unsigned int i = 0; i < edgesFromNode.size(); i++) {
				if(preNode == edgesFromNode[i].first){
					addToSteiner(std::make_pair(preNode, node), edgesFromNode[i].second, localWeights);
					break;
				}
			}

			node = preNode;
			preNode = map[node].second;
		}while(node != startnode);
		localTerminals->erase(std::find(localTerminals->begin(), localTerminals->end(), primeToAdd));
	}

	delete dijsktra;
	delete localWeights;
	delete localTerminals;
	delete vertexToEdges;
}

