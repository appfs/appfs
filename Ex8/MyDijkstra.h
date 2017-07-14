/**
 * @file 	MyDijkstra.h
 * @author 	Fabian Wegscheider
 * @date 	Jul 10, 2017
 */

#ifndef SRC_MYDIJKSTRA_H_
#define SRC_MYDIJKSTRA_H_

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>

using namespace boost;

using Graph = adjacency_list<vecS, vecS, undirectedS,
		no_property, property<edge_weight_t, double>>;


class MyDijkstra {
public:

	/**
	 * My own implementation of Dijkstra using a fibonacci heap from boost and
	 * also keeping track of the predecessors
	 * @param g the graph which is an adjacency_list from boost
	 * @param numVertices the number of vertices in g
	 * @param source the source node for Dijkstra
	 * @param distances a double array in which the results are stored
	 * @param predecessors an int array in which the predecessors are stored
	 */
	static void computeShortestPaths(Graph& g, int numVertices, int source,
			double distances[], int predecessors[]);

private:
	MyDijkstra() {}
};




#endif /* SRC_MYDIJKSTRA_H_ */
