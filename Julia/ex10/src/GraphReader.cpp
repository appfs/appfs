/*
 * GraphReader.cpp
 *
 *	\brief     Reads in a graph from given file and offers methods to get informations about the graph
 *  \details   Class for reading in a graph file and store it in Edges.
 *  \author    Julia Baumbach
 *  \date      15.07.2017
 */

#include "GraphReader.h"

bool GraphReader::readDataFromFile(char* path){
	ifstream infile;
	infile.open(path, ios::in);
	if (!infile){
		return false;
	}
	string line;
	stringstream s;

	//Read first line
	if (getline(infile, line)){
		s.str(line);
		s >> numberVertices >> numberEdges;
	} else {
		return false;
	}

	//Reading the edge data
	sortedEdges.resize(numberVertices + 1);
	weightMap.resize(numberVertices + 1);

	int startEdge;
	int endEdge;
	int weight;

	while (getline(infile, line)){
		if (!line.empty()){
			s.clear();
			s.str(line);
			s >> startEdge >> endEdge >> weight;
			weightMap.push_back(weight);
			for (unsigned int i = 0; i < numberEdges + 1; i++){
				sortedEdges.at(startEdge).push_back(std::make_pair(endEdge, weight));
				sortedEdges.at(endEdge).push_back(std::make_pair(startEdge, weight));
			}
		}
	}
	return true;
}

SortedEdges GraphReader::getSortedEdges(){
	return sortedEdges;
}

WeightMap GraphReader::getWeightMap(){
	return weightMap;
}

unsigned int GraphReader::getNumberOfEdges(){
	return numberEdges;
}

unsigned int GraphReader::getNumberOfVertices(){
	return numberVertices;
}



