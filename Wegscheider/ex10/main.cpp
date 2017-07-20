/**
 * @file 	main.cpp
 * @author 	Fabian Wegscheider
 * @date 	Jul 19, 2017
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

	//default values for unspecified options
	int N_THREADS = 1;
	int N_TERMINALS = 100;

	/*parsing command line options*/
	try {
		po::options_description desc("Allowed options");
		desc.add_options()
				("help,h", "produce help message")
				("showtree,s", "print tree")
				("threads,t", po::value<int>(&N_THREADS)->default_value(1),
						"set number of threads")
				("terminals,n", po::value<int>(&N_TERMINALS)->default_value(100),
						"set number of starting terminals")
				("input-file", po::value<string>(), "input file");

		po::positional_options_description p;
		p.add("input-file", 1);
		p.add("terminals", -1);
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
			cerr << "Usage: ./ex10 <file.gph> (<number of starting terminals>) "
					<< "(other options)" << endl;
			cerr << "run with -h to see information about options" << endl;
			exit(EXIT_FAILURE);
		}

		printTreeSelected = vm.count("showtree");

	} catch (...) {
		cerr << "Usage: ./ex10 <file.gph> (<number of starting terminals>) "
				<< "(other options)" << endl;
		cerr << "run with -h to see information about options" << endl;
		exit(EXIT_FAILURE);
	}
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
	const Graph g(edges, edges + numEdges , weights, numVertices);


	//all primes in {2,...,numVertices} are considered to be terminals
	vector<int> terminals = PrimeNumbers::findPrimes(numVertices);

	timer::cpu_timer algoTimer;

	int iterations = std::min((int) terminals.size(), N_TERMINALS);
	double objectiveValues[iterations];

	//In these arrays the tree is implicitly stored by remembering the
	//predecessor of each vertex, a -1 means that the vertex is not in the tree
	vector<int*> treePredecessors(iterations);
	for (int i = 0; i < iterations; ++i) {
		treePredecessors[i] = new int[numVertices];

	}

#pragma omp parallel for schedule(dynamic) num_threads(N_THREADS)
	for (int i = 0; i < iterations; ++i) {
		objectiveValues[i] = SteinerTreeHeuristic::computeSteinerTree(g,
			numVertices, treePredecessors[i], terminals, terminals[i]);

	}

	double minValue = std::numeric_limits<double>::infinity();
	int minIdx = -1;

	for (int i = 0; i < iterations; ++i) {
		if (objectiveValues[i] < minValue) {
			minValue = objectiveValues[i];
			minIdx = i;
		}
	}

	string algoTime = algoTimer.format(3, "%w");

	//to this string all edges in the tree are printed if user chose option -s
	string edgeStr;
	if (printTreeSelected) {
		assert(SteinerTreeHeuristic::testAndPrintTree(g, numVertices,
				treePredecessors[minIdx], terminals, terminals[minIdx], edgeStr));
	} else {
		assert(SteinerTreeHeuristic::testTree(g, numVertices,
				treePredecessors[minIdx], terminals, terminals[minIdx]));
	}


	cout << "TLEN: " << minValue << endl;
	if (printTreeSelected) cout << "TREE: " << edgeStr << endl;
	cout << "TIME: " << overallTimer.format(3, "%t") << endl;
	cout << "WALL: " << algoTime << endl;
	cout << endl;

	delete[] edges;
	delete[] weights;
	for (int i = 0; i < iterations; ++i) {
		delete[] treePredecessors[i];
	}

	exit(EXIT_SUCCESS);
}




