/*
 * TreeChecker.cpp
 *
 *  Created on: Jul 11, 2017
 *      Author: july
 */

#include "TreeChecker.h"

TreeChecker::TreeChecker(Edges edges, int numberVertices):
	numberVertices(numberVertices){
	//sort the edges in a more efficient data structure
	sortedEdges.resize(numberVertices);
	for (unsigned int i = 0; i < edges.size(); i++){
		sortedEdges.at(edges.at(i).first).push_back(edges.at(i).second);
		sortedEdges.at(edges.at(i).second).push_back(edges.at(i).first);
	}
}

bool TreeChecker::checkCircles(int startNode){
	return false;
}

bool TreeChecker::checkConnected(int startNode){
	return false;
}


