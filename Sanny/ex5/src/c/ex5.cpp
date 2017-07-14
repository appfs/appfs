/**
 * Exercise 5 : Read in a gph-file, computes the longest (with respect to the edge weights) shortest path from any vertex to the vertex with index 1.
 *
 * @author FirstSanny
 */

#include <iostream>
#include <iomanip>
#include <fstream>
#include <utility>
#include <vector>
#include <climits>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/named_function_params.hpp>
#include <boost/program_options.hpp>
#include <boost/timer/timer.hpp>
#include "Dijkstra.h"

// Constants
namespace {

const char* FILEEND = ".gph";

}

// declaring print
using std::cout;
using std::endl;
using std::cerr;
using std::string;

// declaring types
namespace po = boost::program_options;
using Graph = boost::adjacency_list<boost::listS, boost::vecS,
  boost::undirectedS, boost::no_property,
  boost::property<boost::edge_weight_t, double>> ;
using VertexDescriptor = boost::graph_traits < Graph >::vertex_descriptor;
using Directions = std::vector<VertexDescriptor >;
using Edge = std::pair<int, int >;
using Edges = std::vector<Edge >;
using Weights = std::vector<double >;

po::variables_map parseCommandLine(po::options_description desc, int argn,
		char* argv[]) {
	desc.add_options()("help", "produce help message")("algo,m",
			po::value<int>(),
			"algorithm to solve shortest Path. \n 1 - libary (boost)\n 2 - own Algo")(
			"input-file", po::value<string>(), "input file");
	po::positional_options_description p;
	p.add("input-file", -1);
	po::variables_map vm;
	po::store(
			po::command_line_parser(argn, argv).options(desc).positional(p).run(),
			vm);
	po::notify(vm);
	return vm;
}

char parseAlgoOption(const po::variables_map& vm, char useOwnAlgo) {
	if (vm.count("algo")) {
		useOwnAlgo = vm["algo"].as<int>();
		useOwnAlgo--;
	}
	if (useOwnAlgo != 1 && useOwnAlgo != 0) {
		cout << "No valid Option was given for Algorithm! Fallback: algo from libary" << endl;
		useOwnAlgo = 0;
	}
	if (useOwnAlgo) {
		cout << "Using own algorithm..." << endl;
	} else {
		cout << "Using algorithm from boost-libary..." << endl;
	}
	return useOwnAlgo;
}

/** Reading in a Graphfile, computes the longest shortest path */
int main(int argn, char *argv[]) {
	boost::timer::auto_cpu_timer t;
	if (argn <= 1) {
		cerr << "ERROR : There was no filename" << endl;
		return 1;
	}

	po::options_description desc("Allowed options");
	po::variables_map vm = parseCommandLine(desc, argn, argv);

	if (vm.count("help")) {
	    cout << desc << "\n";
	    return 1;
	}
	char useOwnAlgo = 0;
	useOwnAlgo = parseAlgoOption(vm, useOwnAlgo);

	std::ifstream fileStream;
	if(vm.count("input-file") == 0){
		cerr << "No input-file was given!" << endl;
		return 1;
	}

	string filename = vm["input-file"].as<string >();
	filename += FILEEND;
	cout << "Going to parse the file " << filename << endl;
	fileStream.open(filename.c_str(), std::ios::in);

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
		double weight;
		int count = sscanf(line.c_str(), "%d %d %lf", &start, &end, &weight);
		if (count != 3) {
			line.clear();
			continue;
		}
		edges->push_back(std::make_pair(start, end));
		weights->push_back(weight);
		line.clear();
	}

	Weights weightsForShortestpath;

	if(useOwnAlgo){
		cout << "Compute shortest paths via Dijkstra(own)..." << endl;
		Dijkstra* d = new Dijkstra();
		weightsForShortestpath = d->dijkstra(vertexCount, *edges, *weights, 1);
		delete d;
	} else {

		cout << "Creating graph..." << endl;
		Graph g{edges->begin(), edges->end(), weights->begin(), vertexCount};

		Directions* directions = new std::vector<VertexDescriptor>(vertexCount);
		cout << "Compute shortest paths via Dijkstra(boost)..." << endl;
		Weights* weightMapPointer = new Weights(vertexCount);
		boost::dijkstra_shortest_paths(g, 1,
			boost::predecessor_map(//
					boost::make_iterator_property_map(directions->begin(), boost::get(boost::vertex_index, g))) //
				.distance_map(//
					boost::make_iterator_property_map(weightMapPointer->begin(), boost::get(boost::vertex_index, g))));
		weightsForShortestpath = *weightMapPointer;
		delete weightMapPointer;
		delete directions;
	}

	cout << "Search longest shortest path..." << endl;
	int vertex = 1;
	double distance = 0.;

	for(unsigned int i=2; i<vertexCount; i++){
		if(weightsForShortestpath[i]>distance){
			distance = weightsForShortestpath[i];
			vertex = i;
		}
	}

	cout << "RESULT VERTEX " << vertex << endl;
	cout << "RESULT DIST " << std::fixed << std::setprecision(2) << distance << endl;

	delete edges;
	delete weights;

	return 0;
}
