/*
 * GraphReader.cpp
 *
 *	\brief     Reads in a graph from given file and offers methods to get informations about the graph
 *  \details   Class for reading in a graph file and store it in Edges.
 *  \author    Julia Baumbach
 *  \date      15.07.2017
 */

#include "GraphReader.h"

/**
 * \fn  GraphReader()
 * \brief Constructor for GraphReader Instance
 */
GraphReader::GraphReader(){
	weightMap = new vector<int>();
	sortedEdges = new vector<vector<DijkstraPair> >();
}

/*
 * \fn ~GraphReader()
 * \brief deconstructor for GraphReader instance
 */
GraphReader::~GraphReader(){
	delete sortedEdges;
	delete weightMap;
}

/*
 * \fn readDataFromFile(char* path)
 * \brief reads in graph data from a given path and sort edges and weights in an useful way
 * \return true if reading was successful, otherwise false
 */
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
	sortedEdges->resize(numberVertices + 1, vector<DijkstraPair>());

	int startEdge;
	int endEdge;
	int weight;

	while (getline(infile, line)){
		if (!line.empty()){
			s.clear();
			s.str(line);
			s >> startEdge >> endEdge >> weight;
			weightMap->push_back(weight);
			sortedEdges->at(startEdge).push_back(make_pair(endEdge, weight));
			sortedEdges->at(endEdge).push_back(make_pair(startEdge, weight));
		}
	}
	return true;
}

/*
 * \fn SortedEdges getSortedEdges()
 * \brief returns sortedEdges
 * \return *sortedEdges
 */
SortedEdges GraphReader::getSortedEdges(){
	return *sortedEdges;
}

/*
 * \fn WeightMap getWeightMap()
 * \brief returns weightMap
 * \return *weightMap
 */
WeightMap GraphReader::getWeightMap(){
	return *weightMap;
}

/*
 * \fn unsigned int getNumberOfEdges()
 * \brief returns numberEdges
 * \return numberEdges
 */
unsigned int GraphReader::getNumberOfEdges(){
	return numberEdges;
}

/*
 * \fn unsigned int getNumberOfVertices()
 * \brief returns numberVertices
 * \return numberVertices
 */
unsigned int GraphReader::getNumberOfVertices(){
	return numberVertices;
}



