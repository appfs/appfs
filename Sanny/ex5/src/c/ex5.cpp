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
typedef boost::graph_traits < graph >::vertex_descriptor vertex_descriptor;

int readFirstLine(int edgeCount, string& line) {

	return edgeCount;
}

/** add's fileending and opens the via ifstream */
std::ifstream openFile(char* argv[]) {
	string filename = argv[1];
	filename += FILEEND;
	cout << "Going to parse the file " << filename << endl;
	std::ifstream fileStream;
	fileStream.open(filename.c_str(), std::ios::in);
	return fileStream;
}

/** Creates a Map for boost, cause a normal vector.begin() doesn't work */
boost::iterator_property_map<__gnu_cxx:: __normal_iterator <int*,std::vector<int,std::allocator<int> > > ,boost::vec_adj_list_vertex_id_map<boost::no_property,unsigned long int> ,int,int&> getBoostMap(std::vector<int> directions, graph& g)
{
	return boost::make_iterator_property_map(directions.begin(), boost::get(boost::vertex_index, g));
}

/** Reading in a Graphfile, computes the longest shortest path */
int main(int argn, char *argv[]) {

	if (argn <= 1) {
		cerr << "ERROR : There was no filename" << endl;
		return 1;
	}

	std::ifstream fileStream = openFile(argv);

	if ( (fileStream.rdstate()) != 0 ){
		std::perror("ERROR : Encoutered Problem opening file");
		return 1;
	}

	string line;

	int edgeCount;
	int vertexCount;
	if(std::getline(fileStream, line)){
		sscanf(line.c_str(), "%d %d", &vertexCount, &edgeCount);
		cout << "Vertexcount: " << vertexCount << endl;
		cout << "Edgecount: " << edgeCount << endl;
		line.clear();
	} else {
		cerr << "ERROR : File was empty" << endl;
		return 1;
	}

	std::vector<std::pair<int, int>> edges;
	std::vector<int> weights;

	cout << "Reading edges..." << endl;
	while (getline(fileStream, line)) {
		int start;
		int end;
		int weight;
		int count = sscanf(line.c_str(), "%d %d %d", &start, &end, &weight);
		if (count != 3) {
			line.clear();
			continue;
		}
		edges.push_back(std::make_pair(start, end));
		weights.push_back(weight);
		line.clear();
	}

	cout << "Creating graph..." << endl;
	graph g{edges.begin(), edges.end(), weights.begin(), 7};

	std::vector<vertex_descriptor> directions;
	std::vector<int> weightMap;

	cout << "Compute shortest paths via Dijkstra..." << endl;
	boost::dijkstra_shortest_paths(g, 1,
		boost::predecessor_map(//
				boost::make_iterator_property_map(directions.begin(), boost::get(boost::vertex_index, g))) //
			.distance_map(//
				boost::make_iterator_property_map(weightMap.begin(), boost::get(boost::vertex_index, g))));

	cout << "Search longest shortest path..." << endl;
	int vertex = 1;
	int distance = 0;
	for(int i=2; i<=vertexCount; i++){
		cout << "distance " << weightMap[i] << " from vertex "<< i << '\n';
		if(weightMap[i]>distance){
			distance = weightMap[i];
			vertex = i;
		}
	}

	cout << "RESULT VERTEX " << vertex << '\n';
	cout << "RESULT DIST " << distance << '\n';


	return 0;
}
