/**
 * @file 	SteinerTreeHeuristic.h
 * @author 	Fabian Wegscheider
 * @date 	Jul 14, 2017
 */

#ifndef SRC_STEINERTREEHEURISTIC_H_
#define SRC_STEINERTREEHEURISTIC_H_

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>

using namespace boost;
using std:: vector;
using std::string;

using Graph = adjacency_list<vecS, vecS, undirectedS,
		no_property, property<edge_weight_t, double>>;



class SteinerTreeHeuristic {

public:

	/**
	 * Computes a Steiner Tree on an undirected graph using an improved version
	 * of the shortest-path-heuristic based on Dijkstra.
	 * @param g an undirected Graph (adjacency list from boost)
	 * @param numVertices the number of vertices in g
	 * @param treePredecessors the array in which the Steiner Tree is stored implicitly
	 * @param terminals a vector of vertex indices which are considered terminals
	 * @param root the vertex from which to start the heuristic
	 * @return the objective value of the constructed tree or -1 if input is invalid
	 */
	static double computeSteinerTree(Graph& g, int numVertices, int treePredecessors[],
			vector<int>& terminals, int root);

	/**
	 * Tests whether an array of integers implicitly represents a Steiner Tree
	 * on a given undirected graph by interpreting each entry as predecessor
	 * of the vertex with the respective index. Also prints all edges in the tree.
	 * @param g an undirected Graph (adjacency list from boost)
	 * @param numVertices the number of vertices in g
	 * @param treePredecessors the array which is to be tested
	 * @param terminals the terminals of g
	 * @param root the root from which the tree was constructed
	 * @param edgeString the string to which all edges are printed
	 * @return whether the array represents a Steiner Tree
	 */
	static bool testAndPrintTree(Graph& g, int numVertices, int treePredecessors[],
			vector<int>& terminals, int root, string& edgeString);

	/**
	 * Tests whether an array of integers implicitly represents a Steiner Tree
	 * on a given undirected graph by interpreting each entry as predecessor
	 * of the vertex with the respective index.
	 * @param g an undirected Graph (adjacency list from boost)
	 * @param numVertices the number of vertices in g
	 * @param treePredecessors the array which is to be tested
	 * @param terminals the terminals of g
	 * @param root the root from which the tree was constructed
	 * @return whether the array represents a Steiner Tree
	 */
	static bool testTree(Graph& g, int numVertices, int treePredecessors[],
			vector<int>& terminals, int root);

private:
	SteinerTreeHeuristic() {}
};

#endif /* SRC_STEINERTREEHEURISTIC_H_ */
