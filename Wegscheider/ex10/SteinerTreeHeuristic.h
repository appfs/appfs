/**
 * @file 	SteinerTreeHeuristic.h
 * @author 	Fabian Wegscheider
 * @date 	Jul 14, 2017
 */

#ifndef SRC_STEINERTREEHEURISTIC_H_
#define SRC_STEINERTREEHEURISTIC_H_

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/compressed_sparse_row_graph.hpp>

using namespace boost;
using std::vector;
using std::string;

using weight_type = double;

using CSR_Graph = compressed_sparse_row_graph<directedS, no_property,
		property<edge_weight_t, weight_type>>;



namespace SteinerTreeHeuristic {

	/**
	 * Computes a Steiner Tree on an undirected graph using an improved version
	 * of the shortest-path-heuristic based on Dijkstra.
	 * @param g an undirected Graph (CSR-graph from boost)
	 * @param numVertices the number of vertices in g
	 * @param tree_preds the array in which the tree is stored implicitly.
	 * It is expected to be filled with -1.
	 * @param terminals a vector of vertex indices which are considered terminals
	 * @param root the vertex from which to start the heuristic
	 * @return the objective value of the constructed tree
	 */
	weight_type compute_steiner_tree(const CSR_Graph& g, int num_vertices, int root,
			vector<int>& tree_preds, const vector<int>& terminals);

	/**
	 * Prints all edges in a tree which is represented by a predecessor array
	 * to a string. This only works if the array actually represents a tree.
	 * @param g the underlying undirected Graph (CSR-graph from boost)
	 * @param numVertices the number of vertices in g
	 * @param tree_preds the array which is to be printed
	 * @param terminals the terminals of g
	 * @param root the root vertex from which the tree was constructed
	 * @return whether the array represents a Steiner Tree
	 */
	string print_tree(const CSR_Graph& g, int num_vertices, int root,
			vector<int>& tree_preds, vector<int>& terminals);

	/**
	 * Tests whether an array of integers implicitly represents a Steiner Tree
	 * on a given undirected graph by interpreting each entry as predecessor
	 * of the vertex with the respective index.
	 * @param g the underlying undirected Graph (CSR graph from boost)
	 * @param numVertices the number of vertices in g
	 * @param treePredecessors the array which is to be tested
	 * @param terminals the terminals of g
	 * @param root the root from which the tree was constructed
	 * @return whether the array represents a Steiner Tree
	 */
	bool test_tree(const CSR_Graph& g, int num_vertices, int root,
			vector<int>& tree_preds, vector<int>& terminals);

}
#endif /* SRC_STEINERTREEHEURISTIC_H_ */
