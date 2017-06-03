/**
 * Exercise 5 : Read in a gph-file, computes the longest (with respect to the edge weights) shortest path from any vertex to the vertex with index 1.
 *
 * @author FirstSanny
 */

#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <climits>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/named_function_params.hpp>
#include <boost/array.hpp>

namespace {

const char* FILEEND = ".gph";

}

// declaring print
using std::cout;
using std::endl;
using std::cerr;
using std::string;

typedef boost::adjacency_list<boost::listS, boost::vecS,
  boost::undirectedS, boost::no_property,
  boost::property<boost::edge_weight_t, int>> graph;

/** Reading in a Graphfile, computes the longest shortest path */
int main(int argn, char *argv[]) {

	if (argn <= 1) {
		cerr << "ERROR : There was no filename" << endl;
		return 1;
	}

	string filename = argv[1];
	filename += FILEEND;

	cout << "Going to parse the file " << filename <<endl;

	std::ifstream fileStream;
	fileStream.open(filename.c_str(), std::ios::in);

	if ( (fileStream.rdstate()) != 0 ){
		std::perror("ERROR : Encoutered Problem opening file");
		return 1;
	}

	string* line = new string();
	int edgeCount;

	if(std::getline(fileStream, *line)){
		int vertexCount;
		sscanf(line->c_str(), "%d %d", &vertexCount, &edgeCount);
		cout << "Vertexcount: " << vertexCount << endl;
		cout << "Edgecount: " << edgeCount << endl;
		line->clear();
	} else {
		cerr << "ERROR : File was empty" << endl;
		return 1;
	}

	std::vector<std::pair<int, int>> edges;
	std::vector<int> weights;

	int lineCount = 0;
	while (getline(fileStream, *line)){
		int start;
		int end;
		int weight;
		int count = sscanf(line->c_str(), "%d %d %d", &start, &end, &weight);
		if(count != 3){
			line->clear();
			continue;
		}
		edges.push_back(std::make_pair(start, end));
		weights.push_back(weight);
		lineCount++;
	}

	delete line;

	cout << "Creating graph of " << lineCount << " edges" << endl;
	graph g{edges.begin(), edges.end(), weights.begin(), lineCount};

//	int index = -1;
//	int weight = -1;

	for(int i=2; i<=lineCount; i++){

		boost::array<int, 262145> directions ;
		boost::dijkstra_shortest_paths(g, i,
			boost::predecessor_map(directions.begin()));

		int p = 1;
		while (p != i)
		{
		  std::cout << p << '\n';
		  p = directions[p];
		}
		std::cout << p << '\n';

	}

//	for(int i=2; i<=lineCount; i++){
//
//	}

	return 0;
}
