/*
 * TreeChecker.h
 *
 * 	\brief     Class for checking a given tree
 *  \details   Offers different methods to check a given tree
 *  \author    Julia Baumbach
 *  \date      16.07.2017
 */

#include "TreeChecker.h"

/**
 * \fn TreeChecker(Edges, int)
 * \brief Constructor for TreeChecker instance. Sorts the edges in a more efficient way and initialize variables
 */
TreeChecker::TreeChecker(Edges edges, int maxIndexOfVertex){
	orderedEdges.resize(maxIndexOfVertex + 1);
	relevantVertices.resize(maxIndexOfVertex + 1);
	numberOfVisits.resize(maxIndexOfVertex + 1, 1);

	vertices.push_back(edges.at(0).first);
	for(pair<int, int> edge : edges){
		orderedEdges.at(edge.first).push_back(edge.second);
		orderedEdges.at(edge.second).push_back(edge.first);
		relevantVertices.at(edge.first) = true;
		relevantVertices.at(edge.second) = true;
		if(find(vertices.begin(), vertices.end(), edge.first) == vertices.end()){
			vertices.push_back(edge.first);
		}
		if(find(vertices.begin(), vertices.end(), edge.second) == vertices.end()){
			vertices.push_back(edge.second);
		}
	}
}

/*
 * \fn bool allNodesContained(vector<int>)
 * \brief checks if all nodes of the given list are contained in the graph
 * \param vector<int> nodes list of nodes which should be contained
 * \return true if all nodes are contained, otherwise false
 */
bool TreeChecker::allNodesContained(vector<int>* nodes){
	for(int i = 0; i < nodes->size(); i++){
		if(relevantVertices.at(nodes->at(i)) != true){
			return false;
		}
	}
	return true;
}

/*
 * \fn bool hasNoCircles()
 * \brief checks if the given graph has circles or not by doing a BFS
 * \return true if has no circles, otherwise false
 */
bool TreeChecker::hasNoCircles(){
	if(!BFSalreadyDone){
		doBFS();
		BFSalreadyDone = true;
	}
	//Search if any of the relevant vertices is visited twice
	if(find(numberOfVisits.begin(), numberOfVisits.end(), 2) != numberOfVisits.end()){
		return false;
	}
	return true;
}

/*
 * \fn bool isConnected()
 * \brief checks if the given graph is connected or not
 * \return true if the graph is connected, otherwise false
 */
bool TreeChecker::isConnected(){
	if(!BFSalreadyDone){
			doBFS();
			BFSalreadyDone = true;
	}
	//Search if any of the relevant vertices is never visited
	if(find(numberOfVisits.begin(), numberOfVisits.end(), 0) != numberOfVisits.end()){
		return false;
	}
	return true;
}

/*
 * \fn void doBFS()
 * \brief computes a breath first search on the given graph and counts the visits of the relevant vertices
 */
void TreeChecker::doBFS(){
	for(int vertex : vertices){
		numberOfVisits.at(vertex) = 0;
	}
	vector<int> verticesToVisit;
	verticesToVisit.push_back(vertices.at(0));

	int currNode;

	while(!verticesToVisit.empty()){

		currNode = verticesToVisit.at(0);
		verticesToVisit.erase(verticesToVisit.begin());
		numberOfVisits.at(currNode)++;

		for(int endVertex : orderedEdges.at(currNode)){
			verticesToVisit.push_back(endVertex);
			orderedEdges.at(endVertex)//
				.erase(//
				find(orderedEdges.at(endVertex).begin(), orderedEdges.at(endVertex).end(), currNode)//
			);
		}
	}
}

