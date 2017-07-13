/*
 * TreeChecker.h
 *
 *  Created on: Jul 11, 2017
 *      Author: july
 */

#ifndef TREECHECKER_H_
#define TREECHECKER_H_

#include <vector>
#include "dijkstra.h"
/*
 * \typedef SortedEdges which safes the all edges belonging to one vertex in an int
 */
using SortEdges = std::vector<std::vector<int> >;

class TreeChecker {
public:
	TreeChecker(Edges, int);
	bool checkCircles(int);
	bool checkConnected(int);

private:
	int numberVertices;
	SortEdges sortedEdges;
};

#endif /* TREECHECKER_H_ */
