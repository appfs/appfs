/*
 * dijkstra.cpp
 *
 *  Created on: Jun 19, 2017
 *      Author: july
 */

#include "dijkstra.h"
#include <climits>
#include <iostream>



dijkstra::dijkstra(std::vector<int> weights, std::vector<std::pair<int, int> > edges):
	weights(weights), edges(edges) {
}

std::vector<int> dijkstra::computeShortestPath(unsigned int numberOfVertices){
	std::vector<int> weightsToVertices(numberOfVertices);

	for (unsigned int i = 0; i < numberOfVertices + 1; ++i) {
		weightsToVertices[i] = INT_MAX;
	}
	//Start in point 1
	weightsToVertices[1] = 0;
	int indexCurrentVertex = 1;

	while(!edges.empty()){
		//find all neighbors and remove neighbor edges from edge list
		std::vector<std::pair<int, int> > neighborList;
		std::vector<int> neighborWeightsList;

		int edgesSize = edges.size();
		int i = 0;
		while(i<edgesSize){
			if (indexCurrentVertex == edges.at(i).first || indexCurrentVertex == edges.at(i).second){
				neighborList.push_back(edges.at(i));
				edges.erase(edges.begin() + i);

				neighborWeightsList.push_back(weights.at(i));
				weights.erase(weights.begin() + i);

				edgesSize--;
				i--;
			}
			i++;
		}


		//Update distances and search for the nearest vertex
		int indexNeighborVertex;
		int indexNearestVertex;
		int lowestWeight = INT_MAX;
		for(int i = 0; i < neighborList.size(); i++){
			if(neighborList.at(i).first == indexCurrentVertex){
				indexNeighborVertex = neighborList.at(i).second;
			}else{
				indexNeighborVertex = neighborList.at(i).first;
			}

			int newDistance = weightsToVertices[indexCurrentVertex] + neighborWeightsList.at(i);
			if(newDistance < weightsToVertices[indexNeighborVertex]){
				weightsToVertices[indexNeighborVertex] = newDistance;
			}
			if(neighborWeightsList.at(i) < lowestWeight){
				indexNearestVertex = indexNeighborVertex;
				lowestWeight = neighborWeightsList.at(i);
			}
		}
		indexCurrentVertex = indexNearestVertex;
	}
	return weightsToVertices;
}
