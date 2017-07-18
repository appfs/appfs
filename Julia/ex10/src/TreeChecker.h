/*
 * TreeChecker.h
 *
 * 	\brief     Class for checking a given tree
 *  \details   Offers different methods to check a given tree
 *  \author    Julia Baumbach
 *  \date      16.07.2017
 */

#ifndef SRC_TREECHECKER_H_
#define SRC_TREECHECKER_H_

#include <vector>
#include <algorithm>
#include <iostream>

/*
 * \typedef Edges which representes all edges in a graph in a list
 */
using Edges = std::vector<std::pair<int, int> >;
using OrderedEdges = std::vector<std::vector<int> >;
using namespace std;

class TreeChecker {
public:
	TreeChecker(Edges, int);
	bool allNodesContained(std::vector<int>&);
	bool hasNoCircles();
	bool isConnected();
	
private:
	OrderedEdges orderedEdges;
	vector<bool> relevantVertices;
	vector<int> vertices;

};

#endif /* SRC_TREECHECKER_H_ */
