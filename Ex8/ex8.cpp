/**
 * @file 	ex8.cpp
 * @author 	Fabian Wegscheider
 * @date 	Jul 10, 2017
 */


#include <iostream>
#include <boost/program_options.hpp>
#include <boost/heap/fibonacci_heap.hpp>
#include <boost/timer/timer.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include "GraphParser.h"
#include "ShortestPathHeuristic.h"
#include "MyDijkstra.h"

using std::ifstream;
using std::string;
using std::cout;
using std::cerr;
using std::endl;
using namespace boost;
namespace po = boost::program_options;

using Graph = adjacency_list<vecS, vecS, undirectedS,
		no_property, property<edge_weight_t, double>>;


/**
 * The main function which reads in a graph from a .gph file, considers all
 * vertices with prime indices as terminals and calculates a steiner tree
 * using the shortest-path-heuristic and Dijkstra.
 * @param numargs number of inputs on command line
 * @param args array of inputs on command line
 * @return whether the program operated successfully
 */
int main(int numargs, char* args[]) {

	/*parsing command line options*/
	po::options_description desc("Allowed options");
	desc.add_options()
			("help,h", "produce help message")
			("input-file", po::value< string >(), "input file");
	po::positional_options_description p;
	p.add("input-file", -1);
	po::variables_map vm;
	po::store(po::command_line_parser(numargs, args).
			options(desc).positional(p).run(), vm);
	po::notify(vm);

	if (vm.count("help")) {
		cout << desc << "\n";
		exit(EXIT_SUCCESS);
	}

	string input;
	if (vm.count("input-file")) {
		input = vm["input-file"].as< string >();
	} else {
		cerr << "please specify an input file in the .gph format" << endl;
		exit(EXIT_FAILURE);
	}
	/*end of parsing command line options*/


	int numVertices;
	int numEdges;


	GraphParser parser(input);
	if (!parser.readSuccessfully) {
		cerr << "file could not be read" << endl;
		exit(EXIT_FAILURE);
	}

	//first line is read to get number of vertices and edges
	if (!parser.readFirstLine(numVertices, numEdges)) {
		cerr << "error while reading file, not the right format" << endl;
		exit(EXIT_FAILURE);
	}

	Edge* edges = new Edge[numEdges];
	double* weights = new double[numEdges];

	//rest of the file is read and parsed to a graph
	if (!parser.read(edges, weights)) {
		cerr << "error while reading file, not the right format" << endl;
		exit(EXIT_FAILURE);
	}

	//timer is started
	timer::cpu_timer t;

	//undirected graph is constructed with all edges and their weights
	Graph g(edges, edges + numEdges , weights, numVertices);

	//here the steiner tree is finally constructed
	double result = ShortestPathHeuristic::constructSteinerTree(g, numVertices);

	cout << "objective value of constructed steiner tree: " << result << endl;
	cout << "running time of the algorithm: " << t.format() << endl;

	delete[] edges;
	delete[] weights;

	exit(EXIT_SUCCESS);
}





