/**
 * @file 	MyDijkstra.cpp
 * @author 	Fabian Wegscheider
 * @date 	Jul 10, 2017
 */


#include <boost/heap/fibonacci_heap.hpp>
#include "MyDijkstra.h"

using std::pair;

using Pair = pair<int, double>;
typedef graph_traits<Graph>::vertex_descriptor Vertex;

typename graph_traits<Graph>::out_edge_iterator out_i, out_end;


/**
 * Data that is stored in one node of a heap. Contains an integer and a double.
 * Comparisons are made by the double, smaller has higher priority
 */
struct heap_data
{
    heap::fibonacci_heap<heap_data>::handle_type handle;
    Pair pair;

    heap_data(Pair p):
        pair(p)
    {}

    bool operator<(heap_data const & rhs) const {
        return pair.second > rhs.pair.second;
    }
};

using Heap = heap::fibonacci_heap<heap_data>;


void MyDijkstra::computeShortestPaths(Graph& g, int numVertices, int source,
		double distances[], int predecessors[]) {


	predecessors[source] = source;

	Heap heap;

	Heap::handle_type *handles = new Heap::handle_type[numVertices];

	//initialization of the heap
	for (int i = 0; i < numVertices; ++i) {
		if (i == source) {
			handles[i] = heap.push(std::make_pair(i,0.));
			distances[i] = 0.;
		} else {
			handles[i] = heap.push(std::make_pair(i, std::numeric_limits<double>::infinity()));
			distances[i] = std::numeric_limits<double>::infinity();
		}
	}

	property_map<Graph, edge_weight_t>::type weights = get(edge_weight, g);
	property_map<Graph, vertex_index_t>::type index = get(vertex_index, g);


	//the actual algorithm
	while (!heap.empty()) {
		Pair min = heap.top().pair;
		heap.pop();
		for (tie(out_i, out_end) = out_edges(*(vertices(g).first+min.first), g);
				out_i != out_end; ++out_i) {
			double tmp = min.second + weights[*out_i];
			int targetIndex = index[target(*out_i, g)];
			if (tmp < distances[targetIndex]) {
				distances[targetIndex] = tmp;
				predecessors[targetIndex] = min.first;
				(*handles[targetIndex]).pair.second = tmp;
				heap.increase(handles[targetIndex]);
			}
		}
	}

	delete[] handles;
}

