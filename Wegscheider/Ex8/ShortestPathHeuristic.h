/**
 * @file 	ShortestPathHeuristic.h
 * @author 	Fabian Wegscheider
 * @date 	Jul 10, 2017
 */

#ifndef SRC_SHORTESTPATHHEURISTIC_H_
#define SRC_SHORTESTPATHHEURISTIC_H_

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

using namespace boost;

using Graph = adjacency_list<vecS, vecS, undirectedS,
		no_property, property<edge_weight_t, double>>;


class ShortestPathHeuristic {
	public:
		/**
		 * Constructs a steiner tree on an undirected graph where all vertices with
		 * prime index are considered as terminals using the Shortest Path Heuristic.
		 * The terminal from which to start can be chosen (indices starting at 0).
		 * @param g an undirected graph with at least two vertices
		 * @param numVertices the number of vertices in g
		 * @param source the terminal that should function as root for the heuristic.
		 * This must be a prime number.
		 * @return the objective value of the constructed steiner tree
		 */
		static double constructSteinerTree(Graph& g, int numVertices, int source);

		/**
		 * Constructs a steiner tree on an undirected graph where all vertices with
		 * prime index are considered as terminals using the Shortest Path Heuristic
		 * and starting from Vertex with index 1 (indices starting at 0).
		 * @param g an undirected graph with at least two vertices
		 * @param numVertices the number of vertices in g
		 * @return the objective value of the constructed steiner tree
		 */
		static double constructSteinerTree(Graph& g, int numVertices);

	private:
		ShortestPathHeuristic() {}
		static std::vector<int> findPrimes(int n);
};



#endif /* SRC_SHORTESTPATHHEURISTIC_H_ */
