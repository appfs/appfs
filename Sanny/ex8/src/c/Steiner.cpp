/*
 * Steiner.cpp
 *
 *  Created on: 06.07.2017
 *      Author: alesan
 */

#include "Steiner.h"

Steiner::Steiner() {
	dijsktra = new Dijkstra();
	nodesInTree = new Nodes();
	steinerEdges = new Edges();
}

Steiner::~Steiner() {
	delete dijsktra;
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

/** Computes all primes less than vertexCount */
Primes Steiner::getPrimes(unsigned int vertexCount) {
	Primes primes = Primes();
	primes.push_back(2);
	for (unsigned int i = 3; i < vertexCount; i++) {
		char isPrime = true;
		for (int prime : primes) {
			if (i % prime == 0) {
				isPrime = false;
			}
		}
		if (isPrime) {
			primes.push_back(i);
		}
	}
	return primes;
}

void Steiner::addToSteiner(Edge edge, unsigned int i, Weights& weights) {
	steinerEdges->push_back(edge);
	steinerWeight += weights[i];
	weights[i] = 0;
}

/** Uses prime numbers for terminals and solves the Steiner-tree problem */
void Steiner::steiner(int vertexCount, Edges& edges, Weights& weights, int startnode) {
	steinerEdges->clear();
	nodesInTree->clear();

	if(vertexCount < startnode){
		return;
	}

	Primes primes = getPrimes(vertexCount);
	if(std::find(primes.begin(), primes.end(), startnode) != primes.end()){
		primes.erase(std::find(primes.begin(), primes.end(), startnode));
	}
	Nodes* nodesInTree = new Nodes();
	nodesInTree->push_back(startnode);

	// while there aren't all primes in the steiner-tree
	while(!primes.empty()){
		//compute distances
		WeightsAndPrenodeMap map = dijsktra->dijkstra(vertexCount, edges, weights, startnode);
		int primeToAdd = 0;
		int distToPrime = INT_MAX;
		//go through all missing primes and find the nearest
		for (unsigned int i = 0; i<primes.size(); i++){
			int prime = primes.at(i);
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
			for (unsigned int i = 0; i < edges.size(); i++) {
				Edge edge = edges.at(i);
				if(edge.first == node && edge.second == preNode){
					addToSteiner(edge, i, weights);
					break;
				}
				if(edge.second == node && edge.first == preNode){
					addToSteiner(edge, i, weights);
					break;
				}
			}

			node = preNode;
			preNode = map[node].second;
		}while(node != startnode);
		primes.erase(std::find(primes.begin(), primes.end(), primeToAdd));
	}

}

