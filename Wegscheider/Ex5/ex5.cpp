/**
 * @file 	ex5.cpp
 * @author 	Fabian Wegscheider
 * @date 	Jun 5, 2017
 */

#include <iostream>
#include <fstream>
#include <boost/timer/timer.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/algorithm/string.hpp>

using namespace std;
using namespace boost;

/**
 * main function which reads a graph from a .gph file, then calculates shortest
 * paths from all vertices to the first vertex with the dijsktra algorithm
 * and prints the furthest vertex together with its distance
 * to the standard output
 * @param numargs number of inputs on command line
 * @param args array of inputs on command line
 * @return whether the function operated successfully
 */
int main(int numargs, char *args[]) {


	timer::cpu_timer t;

	if (numargs != 2) {
		cout << "Usage: " << args[0] << " filename" << endl;
		exit(EXIT_FAILURE);
	}

	ifstream inputFile;
	inputFile.open(args[1]);							//trying to read file
	if (inputFile.fail()) {
		cerr << "file could not be read" << endl;
		exit(EXIT_FAILURE);
	}

	typedef adjacency_list<vecS, vecS, undirectedS,
			no_property, property<edge_weight_t, double>> Graph;
	typedef pair<int,int> Edge;

	int numVertices;
	int numEdges;

	string line;
	getline(inputFile, line);					//first line is read
	vector<string> parts;
	split(parts, line, is_any_of(" "));

	if (parts.size() != 2) {
		cerr << "error in file: first line should consist of two integers!" << endl;
		exit(EXIT_FAILURE);
	}

	try {
		numVertices = stoi(parts[0]);		//information from the first line
		numEdges = stoi(parts[1]);			//are stored
	} catch (...) {
		cerr << "error in file: first line should consist of two integers!" << endl;
		exit(EXIT_FAILURE);
	}

	Edge *edges =  new Edge[numEdges];			//in these arrays all information about
	double *weights = new double[numEdges];		//the edges are stored

	int i = 0;

	//read line by line
	while (getline(inputFile, line)) {
		split(parts, line, is_any_of(" "));
		if (parts.size() != 3) {
			cerr << "error in line " << (i+2) << ": line should consists of "
					"two integers and a double!" << endl;
			exit(EXIT_FAILURE);
		}
		try {
			edges[i] = Edge(stoi(parts[0])-1, stoi(parts[1])-1);
			weights[i] = stod(parts[2]);
		} catch(...) {
			cerr << "error in line " << (i+2) << ": line should consists of "
					"two integers and a double!" << endl;
			exit(EXIT_FAILURE);
		}
		++i;
	}

	//undirected graph is constructed with all edges and their weights
	Graph g(edges, edges + numEdges , weights, numVertices);

	vector<double> distances(numVertices);

	//call of dijsktra
	dijkstra_shortest_paths(g, *(vertices(g).first), distance_map(&distances[0]));

	property_map<Graph, vertex_index_t>::type index = get(vertex_index, g);

	double maxDist = 0;
	int maxIdx = 0;
	graph_traits<Graph>::vertex_iterator it;

	//search for furthest vertex
	for (it = vertices(g).first+1; it != vertices(g).second; ++it) {
		double tmp = distances[*it];
		if (tmp > maxDist) {
			maxDist = tmp;
			maxIdx = index(*it);
		}
	}

	cout << "RESULT VERTEX " << (maxIdx+1) << endl;
	cout << "RESULT DIST " << maxDist << endl;
	cout << endl << t.format() <<  endl;

	exit(EXIT_SUCCESS);
}
