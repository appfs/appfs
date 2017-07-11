/**
 * Exercise 8 : Read in a gph-file, interpretes it as a Steiner-problem and solves it.
 *
 * @author FirstSanny
 */

#include <iostream>
#include <iomanip>
#include <fstream>
#include <utility>
#include <boost/program_options.hpp>
#include <boost/timer/timer.hpp>
#include "Steiner.h"
#include "GraphChecker.h"

// Constants
namespace {

	const char* FILEEND = ".gph";

}

// declaring print
using std::cout;
using std::endl;
using std::flush;
using std::cerr;
using std::string;

// declaring types
namespace po = boost::program_options;

/** Parsing the arguments given via command line */
po::variables_map parseCommandLine(po::options_description desc, int argn,
		char* argv[]) {
	desc.add_options()//
			("help,h", "produce help message")//
			("start_node,sn", po::value<std::vector<int >>(),"node, where to start")//
			("input-file", po::value<string>(), "input file");
	po::positional_options_description p;
	p.add("input-file", 1);
	p.add("start_node", -1);
	po::variables_map vm;
	po::store(
			po::command_line_parser(argn, argv).options(desc).positional(p).run(),
			vm);
	po::notify(vm);
	return vm;
}

/** Reading in a Graphfile, computes the Steiner */
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

	std::ifstream fileStream;
	if(vm.count("input-file") == 0){
		cerr << "No input-file was given!" << endl;
		return 1;
	}

	std::vector<int > startnodes;
	if(vm.count("start_node") == 0){
		cout << "using default startnode 2" << endl;
		startnodes = std::vector<int >();
		startnodes.push_back(2);
	} else {
		startnodes = vm["start_node"].as<std::vector<int >>();
	}


	string filename = vm["input-file"].as<string >();
	if(filename.find(FILEEND) == std::string::npos){
		filename += FILEEND;
	}
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

	cout << "Reading edges..." << flush;
	int i = 0;
	while (getline(fileStream, line)) {
		int start;
		int end;
		double weight;
		int count = sscanf(line.c_str(), "%d %d %lf", &start, &end, &weight);
		if (count != 3) {
			line.clear();
			continue;
		}
		edges->at(i) = std::make_pair(start, end);
		weights->at(i) = weight;
		i++;
		line.clear();
	}
	cout << "done" << endl << endl;

	Steiner** steiners = new Steiner*[startnodes.size()];
	cout << "Solves Steiner problem for startnodes ";
	for(unsigned int i = 0; i < startnodes.size(); i++){
		cout << startnodes[i];
		if(i != startnodes.size() - 1){
			cout << ", ";
		} else {
			cout << endl;
		}
	}
	#pragma omp parallel for
	for(unsigned int i = 0; i < startnodes.size(); i++){
		steiners[i] = new Steiner();
		steiners[i]->steiner(vertexCount, edges, *weights, startnodes[i]);
		cout << "Objective value of Steiner-tree for startnode " << startnodes[i] << ": " << steiners[i]->getWeight() << endl;
	}

	Steiner* s = steiners[0];
	int node = startnodes[0];
	int weight = s->getWeight();
	cout << "Searching the one with least weight..." << flush;
	for(unsigned int i = 0; i < startnodes.size(); i++){
		if(weight > steiners[i]->getWeight()){
			s = steiners[i];
			node = startnodes[i];
			weight = steiners[i]->getWeight();
		}
	}
	cout << "done" << endl;
	cout << "It's the one with startnode " << node << endl << endl;


	cout << "Checking for cycle..." << flush;
	Edges steinerEdges = s->getEdges();
	GraphChecker* checker = new GraphChecker(steinerEdges, s->getNodes());
	if(checker->hasCycle()){
		cout << "failed" << endl;
		return 1;
	}
	cout << "passed" << endl;

	cout << "Checking if graph is connected..." << flush;
	if(!checker->isConnected()){
		cout << "failed" << endl;
		return 1;
	}
	cout << "passed" << endl << endl;

	cout << "Edges:" << endl;
	for(unsigned int i = 0; i < steinerEdges.size(); i++){
		Edge edge = steinerEdges[i];
		cout << edge.first << " " << edge.second << endl;
	}

	delete edges;
	delete weights;
	delete checker;
	for(unsigned int i = 0; i < startnodes.size(); i++){
		delete steiners[i];
	}
	delete [] steiners;

	return 0;
}
