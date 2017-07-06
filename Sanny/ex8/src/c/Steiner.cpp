/*
 * Steiner.cpp
 *
 *  Created on: 06.07.2017
 *      Author: alesan
 */

#include "Steiner.h"

Steiner::Steiner() {
	dijsktra = new Dijkstra();

}

Steiner::~Steiner() {
	delete dijsktra;
}

/** Computes all primes less than vertexCount */
Primes* Steiner::getPrimes(unsigned int vertexCount) {
	Primes* primes = new Primes();
	primes->push_back(2);
	for (unsigned int i = 3; i < vertexCount; i++) {
		char isPrime = true;
		for (int prime : *primes) {
			if (i % prime == 0) {
				isPrime = false;
			}
		}
		if (isPrime) {
			primes->push_back(i);
		}
	}
	return primes;
}

/** Uses prime numbers for terminals and solves the Steiner-tree problem */
unsigned int Steiner::steiner(unsigned int vertexCount, Edges& edges, Weights& weights) {
	if(vertexCount < 2){
		return 0;
	}
	int weight = 0;
	Primes* primes = getPrimes(vertexCount);


	// while there are primes not in the steiner-tree
	while(!primes->empty()){
		//go through all missing primes
		for (int prime : *primes){

		}
	}

	delete primes;
	return weight;
}

