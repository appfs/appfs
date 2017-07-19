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
	const int MAX_FOR_DEFAULT_TERMINALS = 100;
	const int DEFAULT_THREAD_NUMBER = 8;
}


// declaring print
using std::cout;
using std::endl;
using std::flush;
using std::cerr;
using std::string;


// declaring types
namespace po = boost::program_options;
using BoostTimer = boost::timer::cpu_timer;
using BoostTimes = boost::timer::cpu_times;
using Primes = std::vector<int >;


/**
 * Computes all primes less than vertexCount
 * by beginning with the prime 2 and trying to divide all numbers smaller than vertexCount
 * and try do divide by all found primes.
 */
Primes getPrimes(unsigned int vertexCount) {
	Primes primes = Primes();
	primes.push_back(2);
	for (unsigned int i = 3; i < vertexCount; i++) {
		char isPrime = true;
		for (int prime : primes) {
			if (i % prime == 0) {
				isPrime = false;
			}
		}
		if (isPrime) {
			primes.push_back(i);
		}
	}
	return primes;
}


/** Parsing the arguments given via command line */
po::variables_map parseCommandLine(po::options_description desc, int argn,
		char* argv[]) {
	desc.add_options()//
			("help,h", "produce help message")//
			("start_nodes,s", po::value<std::vector<int >>(),"nodes, where to start")//
			("thread_number,t", po::value<int>(), "number of threads for parallelization")//
			("input_file,i", po::value<string>(), "input file");
	po::positional_options_description p;
	p.add("input_file", -1);
	po::variables_map vm;
	po::store(
			po::command_line_parser(argn, argv).options(desc).positional(p).run(),
			vm);
	po::notify(vm);
	return vm;
}


/** Reading in a Graphfile, computes the Steiner */
int main(int argn, char *argv[]) {
	BoostTimer timerProgram;
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
	if(vm.count("input_file") == 0){
		cerr << "No input-file was given!" << endl;
		return 1;
	}

	string filename = vm["input_file"].as<string >();
	if(filename.find(FILEEND) == std::string::npos){
		filename += FILEEND;
	}
	cout << "Going to parse the file " << filename << endl;
	fileStream.open(filename.c_str(), std::ios::in);

	if ( (fileStream.rdstate()) != 0 ){
		std::perror("ERROR : Encoutered Problem opening file");
		return 1;
	}

	std::vector<int > startnodes;
	if(vm.count("start_nodes") == 0){
		cout << "Using default startnodes (first 100 primes)" << endl;
	} else {
		startnodes = vm["start_node"].as<std::vector<int >>();
	}

	int threadNumber;
	if(vm.count("thread_number") == 0){
		cout << "Using default number of threads (8)" << endl;
		threadNumber = DEFAULT_THREAD_NUMBER;
	} else {
		threadNumber = vm["thread_number"].as<int >();
	}

	cout << endl;

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

	Primes terminals = getPrimes(vertexCount);
	if(startnodes.empty()){
		for (unsigned int i = 0;
				i < MAX_FOR_DEFAULT_TERMINALS && i < terminals.size(); i++) {
			startnodes.push_back(terminals[i]);
		}
	}

	cout << "Solves Steiner-tree for " << startnodes.size() << " startnodes..." << flush;

	BoostTimer timerSteiner;
	Steiner** steiners = new Steiner*[startnodes.size()];
	#pragma omp parallel for num_threads(threadNumber)
	for(unsigned int i = 0; i < startnodes.size(); i++){
		steiners[i] = new Steiner();
		steiners[i]->computeSteinerTree(vertexCount, edges, *weights, terminals, startnodes[i]);
	}
	timerSteiner.stop();
	BoostTimes timesSteiner = timerSteiner.elapsed();
	cout << "done" << endl;

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
	GraphChecker* checker = new GraphChecker(steinerEdges, s->getNodes(), terminals);
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
	cout << "passed" << endl;

	cout << "Checking if all terminals are included..." << flush;
	if(!checker->containsAllTerminals()){
		cout << "failed" << endl;
		return 1;
	}
	cout << "passed" << endl << endl;

	cout << "TLEN: " << s->getWeight() << endl;

	cout << "TREE:";
	for(unsigned int i = 0; i < steinerEdges.size(); i++){
		Edge edge = steinerEdges[i];
		cout << " (" << edge.first << ", " << edge.second << ")";
	}
	cout << endl;

	timerProgram.stop();
	BoostTimes timesProgram = timerProgram.elapsed();
	cout << "TIME: " << boost::timer::format(timesProgram, 4, "%t") << endl;
	cout << "WALL: " << boost::timer::format(timesSteiner, 4, "%w") << endl;

	delete edges;
	delete weights;
	delete checker;
	for(unsigned int i = 0; i < startnodes.size(); i++){
		delete steiners[i];
	}
	delete [] steiners;

	return 0;
}
