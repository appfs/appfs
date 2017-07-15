/*
 * GraphReader.h
 *
 *	\brief     Reads in a graph from given file and offers methods to get informations about the graph
 *  \details   Class for reading in a graph file and store it in Edges.
 *  \author    Julia Baumbach
 *  \date      15.07.2017
 */

#ifndef SRC_GRAPHREADER_H_
#define SRC_GRAPHREADER_H_

#include <fstream>
#include <sstream>
#include <vector>

/*
 * \typedef WeightMap which representes the weight of the i-th edge as the i-th position
 */
using WeightMap = std::vector<int>;
/*
 *\typedef Pair of two ints, first int represents the end-vertex of a edge, second int represents total weight from starting point to end-vertex
 */
using DijkstraPair = std::pair<int, int>;
/*
 * \typedef SortedEdges which safes the all edges with weights belonging to one vertex in a (index, weight) pair
 */
using SortedEdges = std::vector<std::vector<DijkstraPair> >;
using namespace std;

class GraphReader {

public:
	bool readDataFromFile(char*);
	SortedEdges getSortedEdges();
	WeightMap getWeightMap();
	unsigned int getNumberOfEdges();
	unsigned int getNumberOfVertices();

private:
	unsigned int numberVertices;
	unsigned int numberEdges;
	SortedEdges sortedEdges;
	WeightMap weightMap;
};

#endif /* SRC_GRAPHREADER_H_ */
