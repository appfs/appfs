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

#include "DijkstraSolver.h"

class TreeChecker {
public:
	TreeChecker(Edges, int);
	virtual ~TreeChecker();
	bool allNodesContained(std::vector<int>);
	bool hasNoCircles();
	bool isConnected();
	
private:
	Edges edges;
	SortedEdges sortedEdges;

};

#endif /* SRC_TREECHECKER_H_ */
