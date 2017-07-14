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
#include <boost/heap/fibonacci_heap.hpp>
#include <exception>

/**
 * \struct compare_function
 * \brief defines how 2 dijkstra pairs should be compared at fibonacci-heap
 * \param edge1 first edge to compare
 * \param edge2 second edge to compare
 * \return true if the weight of edge2 is smaller than edge1's weight
 */
struct compare_function{
	bool operator()(const DijkstraPair edge1, const DijkstraPair edge2) const{
		return edge1.second > edge2.second;
	}
};

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
 * \brief computes all shortest paths from given start vertex for the initialized dijkstra instance
 * \param unsigned int numberOfVertices Number of Vertices for the graph
 * \return vector of the weights for all shortest paths
 */
void dijkstra::computeShortestPath(unsigned int startNode, WeightMap& weightsToVertices, std::vector<int>& predecessorMap){
	if(startNode > numberOfVertices){
		std::cerr << "Index of StartVertex must be less or equal to number of vertices" << std::endl;
		throw std::exception();
	}
	VisitedMap alreadyVisited(numberOfVertices, false);

	//Start in point startNode
	weightsToVertices[startNode] = 0;
	predecessorMap[startNode] = startNode;
	int currentVertex = startNode;
	int currentDist = 0;

	boost::heap::fibonacci_heap<DijkstraPair, boost::heap::compare<compare_function>> heap;
	//Put the start vertex in the verticesToVisit list
	heap.push(std::make_pair(startNode, 0));

	while(!heap.empty()){
		if (!alreadyVisited.at(heap.top().first)){
			currentVertex = heap.top().first;
			currentDist = heap.top().second;

			heap.pop();

			std::vector<DijkstraPair> currentEdges = sortedEdges.at(currentVertex);

			for (const DijkstraPair pair : currentEdges){
				//Find out the current neighbor vertex and the weight of the current edge
				int neighborVertex = pair.first;
				int currentWeight = pair.second;
				//Search for the right position in verticesToVisit-list. If Vertex is already visited, don't add this edge
				if (!alreadyVisited.at(neighborVertex)){
					heap.push(std::make_pair(neighborVertex, currentDist + currentWeight));
				}

				int currentDistance = currentDist + currentWeight;

				//Update the predecessor and weightsToVerices maps
				if (currentDistance < weightsToVertices[neighborVertex]){
					weightsToVertices[neighborVertex] = currentDistance;
					predecessorMap[neighborVertex] = currentVertex;
				}
			}
			//Current vertex is visited now
			alreadyVisited.at(currentVertex) = true;
		} else {
			heap.pop();
		}
	}
}

/*
 * \fn void updateEdgeWeight(int edgeStart, int edgeEnd)
 * \brief sets EdgeWeight of edge (edgestart, edgeEnd) to 0
 */
int dijkstra::setEdgeWeightToZero(int edgeStart, int edgeEnd){
	std::vector<DijkstraPair> edges = sortedEdges[edgeStart];
	for(DijkstraPair pair : edges){
		if(pair.first == edgeEnd){
			int oldWeight = pair.second;
			pair.second = 0;
			return oldWeight;
		}
	}return 0;
}
