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

unsigned int Steiner::steiner(unsigned int vertexCount, Edges& edges, Weights& weights) {
	return 1;
}

