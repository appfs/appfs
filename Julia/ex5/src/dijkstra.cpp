/*
 * dijkstra.cpp
 *
 * 	\brief     My dijkstra implementation for the fifth exercise
 *  \details   This class computes the longest shortest path for starting point 1.
 *  \author    Julia Baumbach
 *  \date      17.06.2017
 */

#include "dijkstra.h"
#include <climits>
#include <iostream>

using VisitedMap = std::vector<bool>;

/**
 * \fn the constructor
 * \brief initialize a new dijkstra instance while sorting the given edges
 * \param weights Vector of the weights for the given graph
 * \param edges Vector of the edges for the given graph
 * \return the new dijkstra instance
 */
dijkstra::dijkstra(WeightMap weights, Edges edges, unsigned int numberOfVertices):
	numberOfVertices(numberOfVertices) {
	//sort the edges in a more efficient data structure
	sortedEdges.resize(numberOfVertices);
	for (unsigned int i = 0; i < edges.size(); i++){
		sortedEdges.at(edges.at(i).first).push_back(std::make_pair(edges.at(i).second, weights.at(i)));
		sortedEdges.at(edges.at(i).second).push_back(std::make_pair(edges.at(i).first, weights.at(i)));
	}
}

/**
 * \fn vector<int> dijkstra::computeShortestPath
 * \brief computes all shortest paths from vertex 1 for the initialized dijkstra instance
 * \param unsigned int numberOfVertices Number of Vertices for the graph
 * \return vector of the weights for all shortest paths
 */
WeightMap dijkstra::computeShortestPath(){
	WeightMap weightsToVertices(numberOfVertices); //should be returned
	std::vector<int> predecessorMap(numberOfVertices);
	VisitedMap alreadyVisited(numberOfVertices);

	for (unsigned int i = 0; i < numberOfVertices + 1; i++) {
		weightsToVertices[i] = INT_MAX;
		predecessorMap[i] = -1;
	}
	//Start in point 1
	weightsToVertices[1] = 0;
	predecessorMap[1] = 1;
	int currentVertex = 1;

	std::vector<DijkstraPair> verticesToVisit;

	//Initialize for the first edge. Otherwise there would be a nullptr
	verticesToVisit.push_back(sortedEdges.at(1).at(0));
	weightsToVertices.at(sortedEdges.at(1).at(0).first) = sortedEdges.at(1).at(0).second;
	predecessorMap.at(sortedEdges.at(1).at(0).first) = 1;
	sortedEdges.at(1).erase(sortedEdges.at(1).begin());


	while(!verticesToVisit.empty()){
		if (!alreadyVisited.at(currentVertex)){

			std::vector<DijkstraPair> currentEdges = sortedEdges.at(currentVertex);

			for (unsigned int i = 0; i<currentEdges.size(); i++){
				//Search for the right position in verticesToVisit-list. If Vertex is already visited, don't add this edge
				if (!alreadyVisited.at(currentEdges.at(i).first)){
					unsigned int pos = 0;
					while (pos < verticesToVisit.size() && verticesToVisit.at(pos).second < currentEdges.at(i).second){
						pos++;
					}
					verticesToVisit.insert(verticesToVisit.begin() + pos, currentEdges.at(i));

					//Find out the current neighbor vertex and the weight of the current edge
					int neighborVertex = currentEdges.at(i).first;
					int currentWeight = currentEdges.at(i).second;

					int currentDistance = weightsToVertices[currentVertex] + currentWeight;

					//Update the predecessor and weightsToVerices maps
					if (currentDistance < weightsToVertices[neighborVertex]){
						weightsToVertices[neighborVertex] = currentDistance;
						predecessorMap[neighborVertex] = currentVertex;
					}
				}
			}
		}
		//Current vertex is visited now
		alreadyVisited.at(currentVertex) = true;

		currentVertex = verticesToVisit.at(0).first;
		verticesToVisit.erase(verticesToVisit.begin());
	}

	return weightsToVertices;
}
