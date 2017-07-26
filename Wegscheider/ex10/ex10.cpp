/**
 * @file 	main.cpp
 * @author 	Fabian Wegscheider
 * @date 	Jul 19, 2017
 */


#include <iostream>
#include <boost/program_options.hpp>
#include <boost/timer/timer.hpp>
#include <omp.h>
#include "GraphParser.h"
#include "PrimeNumbers.h"
#include "SteinerTreeHeuristic.h"

using std::string;
using std::cout;
using std::cerr;
using std::endl;
using namespace boost;
namespace po = boost::program_options;

using weight_type = double;

using CSR_Graph = compressed_sparse_row_graph<directedS, no_property,
		property<edge_weight_t, weight_type>>;


/**
 * The main function which reads in a graph from a .gph file, considers all
 * vertices with prime indices as terminals and calculates a steiner tree
 * using an improved shortest-path-heuristic based on Dijkstra
 * @param numargs number of inputs on command line
 * @param args array of inputs on command line
 * @return whether the program operated successfully
 */
int main(int numargs, char* args[]) {

	timer::cpu_timer overall_timer;


	//default values for unspecified options
	int N_THREADS = 1;
	int N_TERMINALS = 100;

	/*parsing command line options*/
	string input;
	bool print_tree_selected;

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

		print_tree_selected = vm.count("showtree");

	} catch (...) {
		cerr << "Usage: ./ex10 <file.gph> (<number of starting terminals>) "
				<< "(other options)" << endl;
		cerr << "run with -h to see information about options" << endl;
		exit(EXIT_FAILURE);
	}
	/*end of parsing command line options*/


	/*parsing .gph file*/
	int num_vertices;
	int num_edges;

	GraphParser parser(input);
	if (!parser.opened_successfully) {
		cerr << "file could not be read" << endl;
		exit(EXIT_FAILURE);
	}

	//first line is read to get number of vertices and edges
	if (!parser.read_first_line(num_vertices, num_edges)) {
		cerr << "error while reading file, not the right format" << endl;
		exit(EXIT_FAILURE);
	}

	//we use arrays for edges and weights so that we can use a boost graph
	Edge* edges = new Edge[num_edges];
	weight_type* weights = new double[num_edges];

	//rest of the file is read and parsed to a graph
	if (!parser.read_edge_data(edges, weights)) {
		cerr << "error while reading file, not the right format" << endl;
		exit(EXIT_FAILURE);
	}
	/*end of file parsing*/


	/*initialization and computing*/

	//here the graph is constructed using a CSR graph from boost
	const CSR_Graph g(boost::edges_are_unsorted, &edges[0], &edges[0] + num_edges,
			weights, num_vertices);

	//all primes in {2,...,num_vertices} are considered to be terminals
	vector<int> terminals = PrimeNumbers::find_primes(num_vertices);

	timer::cpu_timer algo_timer;

	const int iterations = std::min((int) terminals.size(), N_TERMINALS);

	//The tree is implicitly stored by remembering the predecessor of each
	//vertex, a -1 means that the vertex is not in the tree
	vector<int> best_tree(num_vertices);

	weight_type min_value = std::numeric_limits<weight_type>::infinity();
	int min_root = -1;

	//heuristic is called for the specified number of terminals and the best
	//solution is kept. It is done in parallel if more than one thread is chosen
	#pragma omp parallel for schedule(dynamic) num_threads(N_THREADS)
	for (int i = 0; i < iterations; ++i) {
		vector<int> tree(num_vertices);
		weight_type objective_value = SteinerTreeHeuristic::compute_steiner_tree(g,
			num_vertices, terminals[i], tree, terminals);

		#pragma omp critical
		if (objective_value < min_value) {
			min_value = objective_value;
			min_root = i;
			best_tree = tree;
		}
	}

	string algo_time = algo_timer.format(3, "%w");
	/*end of computation*/


	assert(SteinerTreeHeuristic::test_tree(g, num_vertices, terminals[min_root],
			best_tree, terminals));

	//output is printed, with tree if option was chosen
	cout << "TLEN: " << min_value << endl;
	if (print_tree_selected) {
		string edge_string = SteinerTreeHeuristic::print_tree(g,
				num_vertices, terminals[min_root], best_tree, terminals);
		cout << "TREE: " << edge_string << endl;
	}
	cout << "TIME: " << overall_timer.format(3, "%t") << endl;
	cout << "WALL: " << algo_time << endl;

	delete[] edges;
	delete[] weights;

	exit(EXIT_SUCCESS);
}




