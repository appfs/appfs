/*
 * TreeChecker.h
 *
 * 	\brief     Class for checking a given tree
 *  \details   Offers different methods to check a given tree
 *  \author    Julia Baumbach
 *  \date      16.07.2017
 */

#include "TreeChecker.h"

TreeChecker::TreeChecker(Edges edges, int numberVertices):
	edges(edges){
	sortedEdges.resize(numberVertices + 1);

}

TreeChecker::~TreeChecker() {
	// TODO Auto-generated destructor stub
}

/*
 * \fn bool allNodesContained(vector<int>)
 * \brief checks if all nodes of the given list are contained in the graph
 * \param vector<int> nodes list of nodes which should be contained
 * \return true if all nodes are contained, otherwise false
 */
bool TreeChecker::allNodesContained(vector<int> nodes){
	for(int node : nodes){
		if(sortedEdges.at(node) == NULL){
			return false;
		}
	} return true;
}

