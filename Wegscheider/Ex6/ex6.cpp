/**
 * @file 	ex6.cpp
 * @author 	Fabian Wegscheider
 * @date 	Jun 20, 2017
 */

#include <iostream>
#include <fstream>
#include <boost/program_options.hpp>
#include <boost/heap/fibonacci_heap.hpp>
#include <boost/timer/timer.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/spirit/include/qi.hpp>

using namespace std;
using namespace boost;
namespace po = boost::program_options;

typedef adjacency_list<vecS, vecS, undirectedS,
		no_property, property<edge_weight_t, double>> Graph;
typedef graph_traits<Graph>::vertex_descriptor Vertex;
typedef pair<int,int> Edge;
typedef pair<int, double> Pair;
typename graph_traits<Graph>::out_edge_iterator out_i, out_end;


/**
 * Data that is stored in one node of a heap. Contains an integer and a double.
 * Comparisons are made by the double, smaller has higher priority
 */
struct heap_data
{
    heap::fibonacci_heap<heap_data>::handle_type handle;
    Pair pair;

    heap_data(Pair p):
        pair(p)
    {}

    bool operator<(heap_data const & rhs) const {
        return pair.second > rhs.pair.second;
    }
};


using Heap = heap::fibonacci_heap<heap_data>;

/**
 * My own implementation of Dijkstra using a fibonacci heap from boost
 * @param g the graph which is an adjacency_list from boost
 * @param numVertices the number of vertices in g
 * @param source the source node for Dijkstra
 * @return vector of resulting distances to source
 */
vector<double> myDijkstra(Graph& g, int numVertices, int source) {

	vector<double> distances(numVertices);
	distances[0] = 0;

	Heap heap;

	Heap::handle_type *handles = new Heap::handle_type[numVertices];
	handles[0] = heap.push(make_pair(0,0.));

	//initialization of the heap
	for (int i = 1; i < numVertices; ++i) {
		handles[i] = heap.push(make_pair(i, numeric_limits<double>::infinity()));
		distances[i] = numeric_limits<double>::infinity();
	}

	property_map<Graph, edge_weight_t>::type weights = get(edge_weight, g);
	property_map<Graph, vertex_index_t>::type index = get(vertex_index, g);


	//the actual algorithm
	while (!heap.empty()) {
		Pair min = heap.top().pair;
		heap.pop();
		for (tie(out_i, out_end) = out_edges(*(vertices(g).first+min.first), g);
				out_i != out_end; ++out_i) {
			double tmp = min.second + weights[*out_i];
			int targetIndex = index[target(*out_i, g)];
			if (tmp < distances[targetIndex]) {
				distances[targetIndex] = tmp;
				(*handles[targetIndex]).pair.second = tmp;
				heap.increase(handles[targetIndex]);
			}
		}
	}

	delete[] handles;
	return distances;
}


/**
 * main function which reads a graph from a .gph file, then calculates shortest
 * paths from all vertices to the first vertex with the dijsktra algorithm
 * and prints the furthest vertex together with its distance
 * to the standard output
 * @param numargs number of inputs on command line
 * @param args array of * inputs on command line
 * @return whether the function operated successfully
 */
int main(int numargs, char *args[]) {

	timer::cpu_timer t;
	bool useOwnMethod;

	/*parsing command line options*/
	po::options_description desc("Allowed options");
	desc.add_options()
			("help,h", "produce help message")
			("m1", "use my own dijkstra method")
			("m2", "use dijkstra method from boost")
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

	if (vm.count("m1") && !vm.count("m2")) {
		useOwnMethod = true;
		cout << "using my own method for calculation..." << endl << endl;
	} else if (vm.count("m2") && !vm.count("m1")) {
		useOwnMethod = false;
		cout << "using boost method for calculation..." << endl << endl;
	} else {
		cerr << "please specify the method you want to use (type -h for help)" << endl;
		exit(EXIT_FAILURE);
	}

	string input;
	if (vm.count("input-file")) {
	    input = vm["input-file"].as< string >();
	} else {
		cerr << "please specify an input file in the .gph format" << endl;
		exit(EXIT_FAILURE);
	}
	/*end of parsing command line options*/


	ifstream inputFile;
	inputFile.open(input);							//trying to read file
	if (inputFile.fail()) {
		cerr << "file could not be read" << endl;
		exit(EXIT_FAILURE);
	}

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

	using namespace boost::spirit;
	using qi::int_;
	using qi::double_;
	using qi::phrase_parse;
	using ascii::space;

	//read line by line using boost to parse each line to int,int,double
	while (getline(inputFile, line)) {
		try {
			auto it = line.begin();
			int start;
			int end;
			double weight;
			bool success = phrase_parse(it, line.end(),
					int_[([&start](int j){ start = j; })]
					>> int_[([&end](int j){ end = j; })]
					>> double_[([&weight](double j){ weight = j; })], space);
			if (success && it == line.end()) {
				edges[i] = Edge(start-1, end-1);
				weights[i] = weight;
			} else {
				cerr << "error in line " << (i+2) << ": line should consists of "
						"two integers and a double!" << endl;
				exit(EXIT_FAILURE);
			}
		} catch(...) {
			cerr << "error in line " << (i+2) << ": line should consists of "
					"two integers and a double!" << endl;
			exit(EXIT_FAILURE);
		}
		++i;
	}

	//undirected graph is constructed with all edges and their weights
	Graph g(edges, edges + numEdges , weights, numVertices);

	//in this vector the resulting distances from dijkstra are stored
	vector<double> distances;

	//call of dijsktra depending on chosen option
	if (!useOwnMethod) {
		distances.resize(numVertices);
		dijkstra_shortest_paths(g, *(vertices(g).first), distance_map(&distances[0]));
	} else {
		distances = myDijkstra(g, numVertices, 0);
	}

	double maxDist = 0;
	int maxIdx = 0;

	//search for furthest vertex
	for (int i = 1; i < numVertices; i++) {
		double tmp = distances[i];
			if (tmp > maxDist) {
				maxDist = tmp;
				maxIdx = i;
			}
	}

	//results are printed to command line
	cout << "RESULT VERTEX " << (maxIdx+1) << endl;
	cout << "RESULT DIST " << maxDist << endl;
	cout << endl << "running time: " << t.format() <<  endl;

	delete[] edges;
	delete[] weights;

	exit(EXIT_SUCCESS);
}



