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

	variables_map vm;
	store(parsed_options, vm);
	notify(vm);

	if (vm.count("help")){
		cout << desc << endl;
	}
	if(vm.count("filename") == 0){
		cerr << "Please enter a file name!" << endl;
		return EXIT_FAILURE;
	}

	vector<int> startNodes;
	if(vm.count("startnode") == 0){
		cout << "Set default start node 2" << endl;
		startNodes.push_back(2);
	}else{
		startNodes = vm["startnode"].as<vector<int> >();
	}

	ifstream infile;
	infile.open(vm["filename"].as<string>(), ios::in);
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

	Steiner mySteiner(numberVertices, edges, weights);

	Edges result;
	int minObjValue = INT_MAX;
	int minStartNode;

	for(int startNode : startNodes){
		Edges temp_result = mySteiner.solve(startNode);
		if (mySteiner.getObjectiveValue() < minObjValue){
			minObjValue = mySteiner.getObjectiveValue();
			result = temp_result;
			minStartNode = startNode;
		}
	}
	cout << "Minimal spanning tree is computed for starting node " << minStartNode << endl;

	cout << "Its edges of the minimal spanning tree are: " << endl;
	for(pair<int, int> result_pair : result){
		cout << result_pair.first << " " << result_pair.second << endl;
	}

	cout << "The objective value of the minimal spanning tree is " << minObjValue << endl;

	return EXIT_SUCCESS;
}

