/*
 * ex5.cpp
 *
 * 	\brief     Fifth exercixe
 *  \details   This class reads graph-data. It computes the longest shortest path for starting point 1.
 *  \author    Julia Baumbach
 *  \date      03.06.2017
 */

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/named_function_params.hpp>
#include <boost/timer/timer.hpp>
#include "dijkstra.h"

using namespace std;

/**
 * \param SOURCE_FILE_PATH default file path if none is hand over while starting the programm
 */
const char* SOURCE_FILE_PATH = "testgraph.gph";

/**
 *\typedef defines a graph by undirected adjacency-list with weighted edges
 */
using graph =  boost::adjacency_list<boost::listS, boost::vecS,
	boost::undirectedS, boost::no_property,
	boost::property<boost::edge_weight_t, int>>;
/*
 * \typedef short version for vertex descriptor from boost::graph_traits
 */
using vertex_descriptor = boost::graph_traits < graph >::vertex_descriptor;

/*
 * Main function. Reads some graph data from a given file and computes the longest
 * shortest path to vertex with number 1.
 */
int main(int argc, char* argv[]){
	//Initialize timer for time measurement
	boost::timer::cpu_timer timer;

	if(argc != 3){
		cerr << "Invalid method call. Please call with ./ex5 -m1/-m2 FILENAME" << endl;
		return EXIT_FAILURE;
	}

	string method = argv[1];
	bool boostMethod = false;
	if (method.compare("-m1") == 0){
		cout << "Using dijkstra from boost-library" << endl;
		boostMethod = true;
	} else if (method.compare("-m2") == 0){
		cout << "Using my dijkstra" << endl;
	} else {
		cerr << "Invalid argument. Please type \"-m1\" for boost-dijkstra or \"-m2\" for manual dijkstra." << endl;
	}


	ifstream infile;
	infile.open(argv[2], ios::in);
	if (!infile){
		cout << "File could not be opened." << endl;
		return 1;
	}

	string line;
	stringstream s;
	unsigned int numberVertices;
	unsigned int numberEdges;

	//Read first line
	if (getline(infile, line)){
		s.str(line);
		s >> numberVertices >> numberEdges;
		//Number of vertices is one more than the real number of vertices for storing all the vertices correctly (it's 1-based)
		numberVertices++;
	} else {
		cerr << "Empty file. Exit program" << endl;
		return 1;
	}

	//Reading the edge data
	Edges edges;
	WeightMap weights;
	int startEdge;
	int endEdge;
	int weight;

	while (getline(infile, line)){
		if (!line.empty()){
			s.clear();
			s.str(line);
			s >> startEdge >> endEdge >> weight;
			edges.push_back(make_pair(startEdge, endEdge));
			weights.push_back(weight);
		}
	}
	infile.close();


	WeightMap weightMap(numberVertices);

	if (boostMethod){
		//Creating a graph g
		graph g{edges.begin(), edges.end(), weights.begin(), numberVertices};

		//storing the shortest paths and its weights
		vector<vertex_descriptor> directions(numberVertices);

		boost::dijkstra_shortest_paths(g, 1,//
				boost::predecessor_map(//
						boost::make_iterator_property_map(directions.begin(), get(boost::vertex_index, g)))//
				.distance_map(//
						boost::make_iterator_property_map(weightMap.begin(), get(boost::vertex_index, g))));
	}
	else {
		dijkstra myDijkstra(weights, edges, numberVertices);
		weightMap = myDijkstra.computeShortestPath(1);
	}

	//Compute the longest shortest path
	int weightOfLongestShortestPath = -1;
	int indexOfVertex = -1;
	int totalWeight;
	for(unsigned int i = 2; i < numberVertices; i++){
		totalWeight = weightMap[i];
		if (totalWeight > weightOfLongestShortestPath){
			weightOfLongestShortestPath = totalWeight;
			indexOfVertex = i;
		}
	}

	cout << "RESULT VERTEX " << indexOfVertex << endl;
	cout << "RESULT DIST " << weightOfLongestShortestPath << endl;

	//Print measured time
	boost::timer::cpu_times times = timer.elapsed();
	cout << "Wall-clock time: " << times.wall * 1e-9 << " seconds" << endl;
	cout << "User-time: " << times.user * 1e-9 <<  " seconds" << endl;

	return 0;
}
