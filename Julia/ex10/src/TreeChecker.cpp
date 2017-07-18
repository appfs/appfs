/*
 * TreeChecker.h
 *
 * 	\brief     Class for checking a given tree
 *  \details   Offers different methods to check a given tree
 *  \author    Julia Baumbach
 *  \date      16.07.2017
 */

#include "TreeChecker.h"

TreeChecker::TreeChecker(Edges edges, int numberVertices){
	orderedEdges.resize(numberVertices + 1);
	relevantVertices.resize(numberVertices + 1);
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
bool TreeChecker::allNodesContained(vector<int>& nodes){
	for(int node : nodes){
		if(relevantVertices.at(node) != true){
			return false;
		}
	}
	return true;
}

bool TreeChecker::hasNoCircles(){
	//Initialize for breath first search
	vector<bool> alreadyVisited(relevantVertices.size(), true);
	for(int vertex : vertices){
		alreadyVisited.at(vertex) = false;
	}

	vector<int> verticesToVisit;
	verticesToVisit.push_back(vertices.at(0));

	int currNode;

	while(!verticesToVisit.empty()){

		currNode = verticesToVisit.at(0);
		verticesToVisit.erase(verticesToVisit.begin());
		alreadyVisited.at(currNode) = true;

		for(int endVertex : orderedEdges.at(currNode)){
			//Check if we already visited this node, we found a circle
			if(alreadyVisited.at(endVertex) == true){
				cout << "Break at " << endVertex << endl;
				return false;
			}
			verticesToVisit.push_back(endVertex);
			orderedEdges.at(endVertex)//
					.erase(//
					find(orderedEdges.at(endVertex).begin(), orderedEdges.at(endVertex).end(), currNode)//
					);
		}
	}
	return true;

}


bool TreeChecker::isConnected(){
	//Initialize for breath first search
	vector<bool> alreadyVisited(relevantVertices.size(), true);
	for(int vertex : vertices){
		alreadyVisited.at(vertex) = false;
	}
	vector<int> verticesToVisit;
	verticesToVisit.push_back(vertices.at(0));

	int currNode;

	while(!verticesToVisit.empty()){
		currNode = verticesToVisit.at(0);
		verticesToVisit.erase(verticesToVisit.begin());
		alreadyVisited.at(currNode) = true;
		for(int endVertex : orderedEdges.at(currNode)){
			verticesToVisit.push_back(endVertex);
//			orderedEdges.at(endVertex)//
//					.erase(//
//					find(orderedEdges.at(endVertex).begin(), orderedEdges.at(endVertex).end(), currNode)//
//					);
		}
	}
	if(find(alreadyVisited.begin(), alreadyVisited.end(), false) != alreadyVisited.end()){
		return false;
	}
	return true;
}

