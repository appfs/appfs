/**
 * @file 	main.cpp
 * @author 	Fabian Wegscheider
 * @date 	Jul 14, 2017
 */


#include <iostream>
#include <boost/program_options.hpp>
#include <boost/timer/timer.hpp>
#include <boost/graph/adjacency_list.hpp>
#include "GraphParser.h"
#include "PrimeNumbers.h"
#include "SteinerTreeHeuristic.h"

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
 * using the an improved shortest-path-heuristic based on Dijkstra
 * @param numargs number of inputs on command line
 * @param args array of inputs on command line
 * @return whether the program operated successfully
 */
int main(int numargs, char* args[]) {

	timer::cpu_timer overallTimer;

	string input;
	bool printTreeSelected;
	int root = 1;

	/*parsing command line options*/
	po::options_description desc("Allowed options");
	desc.add_options()
			("help,h", "produce help message")
			("showtree,s", "print tree")
			("root,r", po::value<int>(&root)->default_value(1),
					"set root node (prime number)")
			("input-file", po::value<string>(), "input file");
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

	if (vm.count("input-file")) {
		input = vm["input-file"].as< string >();
	} else {
		cerr << "please specify an input file in the .gph format" << endl;
		exit(EXIT_FAILURE);
	}

	printTreeSelected = vm.count("showtree");
	/*end of parsing command line options*/


	int numVertices;
	int numEdges;


	GraphParser parser(input);
	if (!parser.openedSuccessfully) {
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
	if (!parser.readEdgeData(edges, weights)) {
		cerr << "error while reading file, not the right format" << endl;
		exit(EXIT_FAILURE);
	}

	//undirected graph is constructed with all edges and their weights
	Graph g(edges, edges + numEdges , weights, numVertices);

	//In this array the steiner tree is implicitly stored by remembering the predecessor
	//of each vertex. A -1 means that the vertex is not contained in the tree
	int* treePredecessors = new int[numVertices];

	//all primes in {2,...,numVertices} are considered to be terminals
	vector<int> terminals = PrimeNumbers::findPrimes(numVertices);

	timer::cpu_timer algoTimer;

	//here the steiner tree is finally constructed
	double objectiveValue = SteinerTreeHeuristic::computeSteinerTree(g, numVertices,
			treePredecessors, terminals, root);

	if (objectiveValue == -1) {
		cerr << "the root you selected is not a valid prime number" << endl;
		exit(EXIT_FAILURE);
	}

	string algoTime = algoTimer.format(3, "%w");

	//to this string all edges in the tree are printed if user chose option -s
	string edgeString;
	if (printTreeSelected) {
		assert(SteinerTreeHeuristic::testAndPrintTree(g, numVertices, treePredecessors,
			terminals, root, edgeString));
	} else {
		assert(SteinerTreeHeuristic::testTree(g, numVertices, treePredecessors,
			terminals, root));
	}


	cout << "TLEN: " << objectiveValue << endl;
	if (printTreeSelected) cout << "TREE: " << edgeString << endl;
	cout << "TIME: " << overallTimer.format(3, "%t") << endl;
	cout << "WALL: " << algoTime << endl;
	cout << endl;

	delete[] edges;
	delete[] weights;
	delete[] treePredecessors;

	exit(EXIT_SUCCESS);
}




