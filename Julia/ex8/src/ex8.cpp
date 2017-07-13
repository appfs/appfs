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

using namespace std;

int main(int argc, char* argv[]){
	if(argc != 3){
			cerr << "Invalid method call. Please call with ./ex5 -m1/-m2 FILENAME" << endl;
			return EXIT_FAILURE;
	}

	ifstream infile;
	infile.open(argv[1], ios::in);
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
	stringstream ss(argv[2]);
	unsigned int startNode;
	ss >> startNode;
	if(startNode > numberVertices || startNode < 1){
		cerr << "Start node must be an int between 1 and " << numberVertices << endl;
		return EXIT_FAILURE;
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

	Steiner mySteiner(numberVertices, edges, weights);
	Edges result = mySteiner.solve(startNode);

	cout << "Edges of the minimal spanning tree are: " << endl;
	for(pair<int, int> result_pair : result){
		cout << result_pair.first << " " << result_pair.second << endl;
	}

	cout << "Objective value of the minimal spanning tree is " << mySteiner.getObjectiveValue() << endl;

	return EXIT_SUCCESS;
}

