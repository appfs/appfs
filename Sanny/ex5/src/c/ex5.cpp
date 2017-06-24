/**
 * Exercise 5 : Read in a gph-file, computes the longest (with respect to the edge weights) shortest path from any vertex to the vertex with index 1.
 *
 * @author FirstSanny
 */

#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <climits>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/named_function_params.hpp>
#include <boost/timer/timer.hpp>
#include "Dijkstra.h"

// Constants
namespace {

const char* FILEEND = ".gph";
const char* OWNALGO = "-m1";
const char* LIBALGO = "-m2";

}

// declaring print
using std::cout;
using std::endl;
using std::cerr;
using std::string;

// declaring types
using Graph = boost::adjacency_list<boost::listS, boost::vecS,
  boost::undirectedS, boost::no_property,
  boost::property<boost::edge_weight_t, int>> ;
using VertexDescriptor = boost::graph_traits < Graph >::vertex_descriptor;
using Directions = std::vector<VertexDescriptor >;
using Edge = std::pair<int, int>;
using Edges = std::vector<Edge >;
using Weights = std::vector<int >;


/** check which method should be used for computing the shortest path */
char checkPreferredAlgo(char* argv[], int argn) {
	if(argn == 2){
		return 0;
	}
	string command = argv[1];
	const string::size_type indexCommandOwnAlgo = command.find(OWNALGO);
	if(indexCommandOwnAlgo != string::npos){
		return 1;
	}
	const string::size_type indexCommandLibAlgo = command.find(LIBALGO);
	if(indexCommandLibAlgo != string::npos){
		return 0;
	}
	return 0;
}

/** add's fileending and opens the via ifstream */
std::ifstream openFile(char* argv[], int argn) {
	string filename;
	if(argn == 2){
		filename = argv[1];
	} else {
		filename = argv[2];
	}
	filename += FILEEND;
	cout << "Going to parse the file " << filename << endl;
	std::ifstream fileStream;
	fileStream.open(filename.c_str(), std::ios::in);
	return fileStream;
}


/** Reading in a Graphfile, computes the longest shortest path */
int main(int argn, char *argv[]) {
	boost::timer::auto_cpu_timer t;
	if (argn <= 1) {
		cerr << "ERROR : There was no filename" << endl;
		return 1;
	}

	char useOwnAlgo = checkPreferredAlgo(argv, argn);
	if(useOwnAlgo){
		cout << "Using own algorithm..." << endl;
	} else {
		cout << "Using algorithm from boost-libary..." << endl;
	}

	std::ifstream fileStream = openFile(argv, argn);


	if ( (fileStream.rdstate()) != 0 ){
		std::perror("ERROR : Encoutered Problem opening file");
		return 1;
	}

	string line;

	unsigned int edgeCount;
	unsigned int vertexCount;

	if(std::getline(fileStream, line)){
		sscanf(line.c_str(), "%d %d", &vertexCount, &edgeCount);
		cout << "Vertexcount: " << vertexCount << endl;
		cout << "Edgecount: " << edgeCount << endl;
		line.clear();
		vertexCount++;
	} else {
		cerr << "ERROR : File was empty" << endl;
		return 1;
	}

	Edges* edges = new Edges(edgeCount);
	Weights* weights = new Weights(edgeCount);

	cout << "Reading edges..." << endl;
	while (getline(fileStream, line)) {
		int start;
		int end;
		int weight;
		int count = sscanf(line.c_str(), "%d %d %d", &start, &end, &weight);
		if (count != 3) {
			line.clear();
			continue;
		}
		edges->push_back(std::make_pair(start, end));
		weights->push_back(weight);
		line.clear();
	}

	cout << "Creating graph..." << endl;
	Graph g{edges->begin(), edges->end(), weights->begin(), vertexCount};

	Directions* directions = new std::vector<VertexDescriptor>(vertexCount);
	Weights weightsForShortestpath;

	if(useOwnAlgo){
		cout << "Compute shortest paths via Dijkstra(own)..." << endl;
		Dijkstra* d = new Dijkstra();
		weightsForShortestpath = d->dijkstra(vertexCount, *edges, *weights, 1);
	} else {
		cout << "Compute shortest paths via Dijkstra(boost)..." << endl;
		std::vector<int>* weightMapPointer = new std::vector<int>(vertexCount);
		boost::dijkstra_shortest_paths(g, 1,
			boost::predecessor_map(//
					boost::make_iterator_property_map(directions->begin(), boost::get(boost::vertex_index, g))) //
				.distance_map(//
					boost::make_iterator_property_map(weightMapPointer->begin(), boost::get(boost::vertex_index, g))));
		weightsForShortestpath = *weightMapPointer;
		delete weightMapPointer;
	}

	cout << "Search longest shortest path..." << endl;
	int vertex = 1;
	int distance = 0;

	for(unsigned int i=2; i<vertexCount; i++){
		if(weightsForShortestpath[i]>distance){
			distance = weightsForShortestpath[i];
			vertex = i;
		}
	}

	cout << "RESULT VERTEX " << vertex << endl;
	cout << "RESULT DIST " << distance << endl;

	delete edges;
	delete weights;
	delete directions;

	return 0;
}
