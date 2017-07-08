/*
 * GraphChecker.cpp
 *
 *  Created on: 08.07.2017
 *      Author: alesan
 */

#include "GraphChecker.h"

GraphChecker::GraphChecker(Edges edges, int nodeCount) {
	this->edges = edges;
	this->nodeCount = nodeCount;
}

GraphChecker::~GraphChecker() {
	// nop
}

char GraphChecker::isConnected(){

	return 1;
}
char GraphChecker::hasCycle(){

	return 0;
}

