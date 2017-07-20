/**
 * @file 	SteinerTreeHeuristic.cpp
 * @author 	Fabian Wegscheider
 * @date 	Jul 14, 2017
 */

#include <sstream>
#include <boost/heap/priority_queue.hpp>
#include "SteinerTreeHeuristic.h"

using std::pair;
typename graph_traits<Graph>::out_edge_iterator it, itEnd;


/**
 * Data that is stored in one node of the heap. Contains an integer and a double.
 * Comparisons are made by the double, smaller has higher priority
 */
struct heapData
{
	pair<int,double> data;

    heapData(pair<int,double> p):
        data(p)
    {}

    bool operator<(heapData const & rhs) const {
        return data.second > rhs.data.second;
    }
};


/*
 * Implementation of the improved Shortest-Path-Heuristic. A standard binary heap
 * from boost is used and keys are never decreased, but instead new nodes are pushed
 * whenever a better path is found. The resulting tree is stored implicitly in
 * treePredecessors.
 */
double SteinerTreeHeuristic::computeSteinerTree(const Graph& g, const int& numVertices,
		int treePredecessors[], const vector<int>& terminals, const int& root) {

	assert(NULL != treePredecessors);

	//terminals are scanned
	int numTerminals = terminals.size();
	bool* isTerminal = new bool[numVertices]();
	for (int i = 0; i < numTerminals; ++i) {
		assert(terminals[i] < numVertices);
		isTerminal[terminals[i]] = true;
	}

	assert(isTerminal[root]);

	heap::priority_queue<heapData> heap;

	double* distances = new double[numVertices];
	int* predecessors = new int[numVertices];
	predecessors[root] = root;
	double treeCost = 0.;

	//initialization of heap, distances and treePredecessors. note: a -1 in
	//treePredecessors means that the vertex is not connected to the tree yet
	for (int i = 0; i < numVertices; ++i) {
		if (i == root) {
			heap.push(std::make_pair(i,0.));
			distances[i] = 0.;
			treePredecessors[i] = root;
		} else {
			heap.push(std::make_pair(i, std::numeric_limits<double>::infinity()));
			distances[i] = std::numeric_limits<double>::infinity();
			treePredecessors[i] = -1;
		}
	}

	property_map<Graph, edge_weight_t>::const_type weights = get(edge_weight, g);
	property_map<Graph, vertex_index_t>::const_type index = get(vertex_index, g);


	int connectedTerminals = 1;


	//here the actual algorithm starts
	while (connectedTerminals < numTerminals) {
		assert(!heap.empty());	//heap should never be empty before tree contains all terminals

		pair<int,double> min = heap.top().data;
		heap.pop();


		//if we scan a terminal, all vertices on shortest path to subtree
		//are added to heap with weight 0 and included into the tree
		if (min.first != root && isTerminal[min.first] && treePredecessors[min.first] == -1) {
			++connectedTerminals;

			min.second = 0.;
			int nextVertex = predecessors[min.first];
			treePredecessors[min.first] = predecessors[min.first];

			auto edgeInfo = edge(predecessors[min.first], min.first, g);
			assert(edgeInfo.second);
			treeCost += weights[edgeInfo.first];

			//here all vertices on path from new terminal to tree are added again with key=0
			while (treePredecessors[nextVertex] == -1) {
				heap.push(std::make_pair(nextVertex, 0.));
				treePredecessors[nextVertex] = predecessors[nextVertex];

				edgeInfo = edge(predecessors[min.first], min.first, g);
				assert(edgeInfo.second);
				treeCost += weights[edgeInfo.first];

				nextVertex = predecessors[nextVertex];
			}
		}

		//this is basically the usual dijkstra with pushes instead of decreasekey operations
		for (tie(it, itEnd) = out_edges(*(vertices(g).first+min.first), g);
				it != itEnd; ++it) {
			double tmp = min.second + weights[*it];
			int targetIndex = index[target(*it, g)];
			//we only consider vertices which have not been added to the tree yet
			if (treePredecessors[targetIndex] == -1 && tmp < distances[targetIndex]) {
				distances[targetIndex] = tmp;
				predecessors[targetIndex] = min.first;
				heap.push(std::make_pair(targetIndex, tmp));
			}
		}
	}

	delete[] isTerminal;
	delete[] distances;
	delete[] predecessors;

	return treeCost;
}



bool SteinerTreeHeuristic::testAndPrintTree(const Graph& g, int numVertices, int treePredecessors[],
		vector<int>& terminals, int root, string& edgeString) {

	std::stringstream stream;
	bool* visited = new bool[numVertices]();

	for (unsigned int i = 0; i < terminals.size(); ++i) {
		int curr = terminals[i];
		int vertexCount = 1;
		int edgeCount = 0;
		while (curr != root && vertexCount != numVertices && curr != -1) {
			if (!visited[curr]) {
				++edgeCount;
				//edges are added to string and every 50 lines a linebreak is
				//added for the sake of readability
				if (edgeCount % 50 == 0) stream << std::endl;
				stream << "(" << curr << "," << treePredecessors[curr] << ") ";
			}
			curr = treePredecessors[curr];
			++vertexCount;
		}

		//if a cycle is found or not all terminals are connected, result is false
		if (vertexCount == numVertices || curr == -1){
			delete[] visited;
			return false;
		}
	}

	edgeString = stream.str();

	delete[] visited;
	return true;
}



bool SteinerTreeHeuristic::testTree(const Graph& g, int numVertices, int treePredecessors[],
		vector<int>& terminals, int root) {

		bool* visited = new bool[numVertices]();

		for (unsigned int i = 0; i < terminals.size(); ++i) {
			int curr = terminals[i];
			int vertexCount = 1;
			int edgeCount = 0;
			while (curr != root && vertexCount != numVertices && curr != -1) {
				if (!visited[curr]) ++edgeCount;
				curr = treePredecessors[curr];
				++vertexCount;
			}

			//if a cycle is found or not all terminals are connected, result is false
			if (vertexCount == numVertices || curr == -1){
				delete[] visited;
				return false;
			}
		}

		delete[] visited;
		return true;
}



