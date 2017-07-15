/*
 * ex10.cpp
 *
 * 	\brief     Tenth exercise
 *  \details   This class reads graph-data and computes the so-called Steiner-tree for the first 100 terminals by multithreading
 *  \author    Julia Baumbach
 *  \date      15.07.2017
 */

#include "Steiner.h"
#include "GraphReader.h"
#include <iostream>
#include <boost/program_options.hpp>
#include <boost/timer/timer.hpp>

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
	//Initialize timer for time measurement
	boost::timer::cpu_timer timer;

	GraphReader reader;
	if(!reader.readDataFromFile(argv[1])){
		cerr << "Error while reading data. Exit program" << endl;
		return EXIT_FAILURE;
	}

	//Get data from graphReader
	unsigned int numberVertices = reader.getNumberOfVertices();
	SortedEdges edges = reader.getSortedEdges();
	WeightMap weights = reader.getWeightMap();

	vector<int> terminals = computePrimes(numberVertices);
	vector<int> firstHundredTerminals(terminals.begin(), terminals.begin() + 100);

	//Solve the Steiner Problem for the given graph and given start nodes
	Steiner mySteiner(terminals);

	Edges result;
	int minObjValue = INT_MAX;
	int minStartNode;

//	for(int startNode : firstHundredTerminals){
		Edges temp_result = mySteiner.solveSteiner(edges, numberVertices, 2);
//		if (mySteiner.getObjectiveValue() < minObjValue){
//			minObjValue = mySteiner.getObjectiveValue();
//			result = temp_result;
//			minStartNode = startNode;
//		}
//	}


	//print results
//	cout << "Minimal spanning tree is computed for starting node " << minStartNode << endl;

	cout << "Its edges of the minimal spanning tree are: " << endl;
	for(pair<int, int> result_pair : temp_result){
		cout << result_pair.first << " " << result_pair.second << endl;
	}

//	cout << "The objective value of the minimal spanning tree is " << minObjValue << endl;

	//Print measured time
	boost::timer::cpu_times times = timer.elapsed();
	cout << "Wall-clock time: " << times.wall * 1e-9 << " seconds" << endl;
	cout << "User-time: " << times.user * 1e-9 <<  " seconds" << endl;

	return EXIT_SUCCESS;
}

