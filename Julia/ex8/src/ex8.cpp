/*
 * ex8.cpp
 *
 * 	\brief     Eigth exercise
 *  \details   This class reads graph-data and computes the so-called Steiner-tree for it
 *  \author    Julia Baumbach
 *  \date      30.06.2017
 */

#include "Steiner.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <boost/program_options.hpp>

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
	options_description desc("");

	desc.add_options()
		("help,h", "Help screen")
	    ("startnodes,s", value<vector<int> >()->multitoken(), "Indizes of the start nodes")
		("filename,f", value<string>(), "Filename for the graph");

	command_line_parser parser{argc, argv};
	parser.options(desc).allow_unregistered().style(
	      command_line_style::default_style |
	      command_line_style::allow_slash_for_short);
	parsed_options parsed_options = parser.run();

	variables_map variables;
	store(parsed_options, variables);
	notify(variables);

	if (variables.count("help")){
		cout << desc << endl;
	}
	if(variables.count("filename") == 0){
		cerr << "Please enter a file name!" << endl;
		return EXIT_FAILURE;
	}

	vector<int> startNodes;
	if(variables.count("startnode") == 0){
		cout << "Set default start node 2" << endl;
		startNodes.push_back(2);
	}else{
		startNodes = variables["startnode"].as<vector<int> >();
	}

	ifstream infile;
	infile.open(variables["filename"].as<string>(), ios::in);
	if (!infile){
		cout << "File could not be opened." << endl;
		return 1;
	}


	string line;
	stringstream s;
	unsigned int numberVertices;
	unsigned int numberEdges;

	//Read first line
	if (getline(infile, line)){
		s.str(line);
		s >> numberVertices >> numberEdges;
		//Number of vertices is one more than the real number of vertices for storing all the vertices correctly (it's 1-based)
		numberVertices++;
	} else {
		cerr << "Empty file. Exit program" << endl;
		return 1;
	}

	//Reading the edge data
	Edges edges;
	WeightMap weights;
	int startEdge;
	int endEdge;
	int weight;

	while (getline(infile, line)){
		if (!line.empty()){
			s.clear();
			s.str(line);
			s >> startEdge >> endEdge >> weight;
			edges.push_back(make_pair(startEdge, endEdge));
			weights.push_back(weight);
		}
	}
	infile.close();

	//Solve the Steiner Problem for the given graph and given start nodes
	Steiner mySteiner(numberVertices, edges, weights, computePrimes(numberVertices));

	Edges result;
	int minObjValue = INT_MAX;
	int minStartNode;

	for(int startNode : startNodes){
		Edges temp_result = mySteiner.solveSteiner(startNode);
		if (mySteiner.getObjectiveValue() < minObjValue){
			minObjValue = mySteiner.getObjectiveValue();
			result = temp_result;
			minStartNode = startNode;
		}
	}

	//print results
	cout << "Minimal spanning tree is computed for starting node " << minStartNode << endl;

	cout << "Its edges of the minimal spanning tree are: " << endl;
	for(pair<int, int> result_pair : result){
		cout << result_pair.first << " " << result_pair.second << endl;
	}

	cout << "The objective value of the minimal spanning tree is " << minObjValue << endl;

	return EXIT_SUCCESS;
}

