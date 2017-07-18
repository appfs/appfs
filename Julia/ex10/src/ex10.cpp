/*
 * ex10.cpp
 *
 * 	\brief     Tenth exercise
 *  \details   This class reads graph-data and computes the so-called Steiner-tree for the first 100 terminals by multithreading
 *  \author    Julia Baumbach
 *  \date      15.07.2017
 */

#include "GraphReader.h"
#include <iostream>
#include <sstream>
#include <boost/program_options.hpp>
#include <boost/timer/timer.hpp>
#include "SteinerSolver.h"
#include "TreeChecker.h"

using namespace std;
using namespace boost::program_options;


/*
 * \fn bool hasDivisor(vector<int>, int)
 * \brief computes if an int has a divisor in a list of ints
 * \return true, if it has a divisor, otherwise false
 */
bool hasDivisor(const vector<int>& result, int i) {
	for (int prime : result) {
		if ((i % prime) == 0 || prime/2 >= i) {
			return true;
		}
	}return false;
}

/*
 * \fn vector<int> computePrimes(int upperBound)
 * \brief computes all primes in range from 2 to upperBound
 * \return vector of all primes
 */
vector<int> computePrimes(int upperBound){
	vector<int> result;
	result.push_back(2);
	for(int i = 3; i < upperBound; i++){
		if(!hasDivisor(result, i)){
			result.push_back(i);
		}
	}
	return result;
}

/*
 * \fn int main(int argc, char* argv[])
 * \brief main function. reads in graph data and prints the solution for the steiner tree problem
 * \return EXIT_SUCCESS if program exited correctly, otherwise EXIT_FAILURE
 */
int main(int argc, char* argv[]){
	//Initialize timer for cpu time measurement
	boost::timer::cpu_timer cpu_timer;

	int numberOfThreads;
	stringstream ss(argv[1]);
	ss >> numberOfThreads;

	GraphReader reader;
	if(!reader.readDataFromFile(argv[2])){
		cerr << "Error while reading data. Exit program" << endl;
		return EXIT_FAILURE;
	}

	//Get data from graphReader
	unsigned int numberVertices = reader.getNumberOfVertices();
	SortedEdges edges = reader.getSortedEdges();
	WeightMap weights = reader.getWeightMap();

	//Initialize timer for wallclock time measurement
	boost::timer::cpu_timer wall_timer;

	vector<int> terminals = computePrimes(numberVertices);
	vector<int> firstHundredTerminals;
	if(terminals.size() < 100){
		firstHundredTerminals = terminals;
	}else {
		firstHundredTerminals = vector<int>(terminals.begin(), terminals.begin() + 100);
	}


	//Solve the Steiner Problem for the given graph and given start nodes
	Edges resultEdges[100];
	int resultObjValues[100];

	#pragma omp parallel for num_threads(numberOfThreads)
	for(int i = 0; i < firstHundredTerminals.size(); i++){
		SteinerSolver* mySteiner = new SteinerSolver(terminals);
		resultEdges[i] = mySteiner->solveSteiner(edges, numberVertices, firstHundredTerminals.at(i));
		resultObjValues[i] = mySteiner->getObjectiveValue();
		delete mySteiner;
	}
	//Stop wall time
	boost::timer::cpu_times wall_time = wall_timer.elapsed();

	//Search for the minimal steiner tree
	int indexMinNode;
	int minObjValue = INT_MAX;
	for(int i = 0; i < firstHundredTerminals.size(); i++){
		if(resultObjValues[i] < minObjValue){
			indexMinNode = i;
			minObjValue = resultObjValues[i];
		}
	}

	//Check if the minimal steiner tree is a tree and contains all terminals
	TreeChecker myChecker(resultEdges[indexMinNode], numberVertices);
	if(!myChecker.allNodesContained(firstHundredTerminals)){
		cout << "CONTAINS NOT ALL TERMINALS" << endl;
	}
	if(!myChecker.hasNoCircles()){
		cout << "CONTAINS CIRLCES" << endl;
	}
	if(!myChecker.isConnected()){
		cout << "NOT CONNECTED" << endl;
	}

	//print results
	cout << "TLEN: " << resultObjValues[indexMinNode] << endl;
	Edges result = resultEdges[indexMinNode];
	cout << "TREE: ";
	for(int i = 0; i < result.size(); i++){
		if(i == result.size() -1){
			cout << "(" << result.at(i).first << "," << result.at(i).second << ")" << endl;
		}else {
			cout << "(" << result.at(i).first << "," << result.at(i).second << ") ";
		}
	}

	//Print measured time
	boost::timer::cpu_times cpu_time = cpu_timer.elapsed();

	cout << "TIME: " << (cpu_time.system + cpu_time.user) * 1e-9 << " seconds" << endl;
	cout << "WALL: " << wall_time.wall * 1e-9 <<  " seconds" << endl;

	return EXIT_SUCCESS;
}

