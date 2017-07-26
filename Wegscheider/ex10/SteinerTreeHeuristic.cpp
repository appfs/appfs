/**
 * @file 	SteinerTreeHeuristic.cpp
 * @author 	Fabian Wegscheider
 * @date 	Jul 14, 2017
 */

#include <sstream>
#include <boost/heap/d_ary_heap.hpp>
#include "SteinerTreeHeuristic.h"

using std::pair;

/*
 * Data that is stored in one node of the heap. Contains an integer and a double.
 * Comparisons are made by the double, smaller has higher priority. The int is
 * used for the vertex index and the double for its key during the algorithm
 */
struct heap_data {
	int index;
	weight_type key;

    heap_data(int i, weight_type k): index(i), key(k) {
    }

    bool operator<(heap_data const & rhs) const {
        return key > rhs.key;
    }
};


/*
 * Implementation of the improved Shortest-Path-Heuristic. A 4-ary-heap from
 * boost is used and keys are never decreased, but instead new nodes are pushed
 * whenever a better path is found. The resulting tree is stored implicitly
 */
double SteinerTreeHeuristic::compute_steiner_tree(const CSR_Graph& g, int num_vertices,
		int root, vector<int>& tree_predecessors, const vector<int>& terminals) {

	assert((int) tree_predecessors.size() == num_vertices);

	//terminals are scanned
	int num_terminals = terminals.size();
	vector<bool> is_terminal(num_vertices, false);
	for (int i = 0; i < num_terminals; ++i) {
		assert(terminals[i] < num_vertices);
		is_terminal[terminals[i]] = true;
	}

	assert(is_terminal[root]);

	heap::d_ary_heap<heap_data, heap::arity<4>> heap;

	vector<weight_type> distances(num_vertices);
	vector<int> predecessors(num_vertices);
	predecessors[root] = root;
	weight_type tree_cost = 0;

	//initialization of heap, distances and treePredecessors. note: a -1 in
	//treePredecessors means that the vertex is not connected to the tree yet
	for (int i = 0; i < num_vertices; ++i) {
		if (i == root) {
			heap.push(heap_data(i,0));

			distances[i] = 0;
			tree_predecessors[i] = root;
		} else {
			heap.push(heap_data(i, std::numeric_limits<weight_type>::infinity()));
			distances[i] = std::numeric_limits<weight_type>::infinity();
			tree_predecessors[i] = -1;
		}
	}


	//this is just boost syntax so that we can access weights and indices later
	property_map<CSR_Graph, edge_weight_t>::const_type weights = get(edge_weight, g);
	property_map<CSR_Graph, vertex_index_t>::const_type index = get(vertex_index, g);

	int connected_terminals = 1;

	//here the actual algorithm starts
	while (connected_terminals < num_terminals) {
		assert(!heap.empty());	//heap should never be empty before tree contains all terminals

		int min_idx = heap.top().index;
		weight_type min_key = heap.top().key;
		heap.pop();


		//if we scan a terminal, all vertices on shortest path to subtree
		//are added to heap with weight 0 and included into the tree
		if (min_idx != root && is_terminal[min_idx] && tree_predecessors[min_idx] == -1) {
			++connected_terminals;

			min_key = 0.;
			int next_vertex = predecessors[min_idx];
			tree_predecessors[min_idx] = next_vertex;

			auto edge_info = edge(vertex(next_vertex,g), vertex(min_idx,g), g);
			assert(edge_info.second);
			tree_cost += weights[edge_info.first];

			//here all vertices on path from new terminal to tree are added again with key=0
			while (tree_predecessors[next_vertex] == -1) {
				heap.push(heap_data(next_vertex, 0));
				tree_predecessors[next_vertex] = predecessors[next_vertex];

				edge_info = edge(vertex(predecessors[min_idx],g), vertex(min_idx, g), g);
				assert(edge_info.second);
				tree_cost += weights[edge_info.first];

				next_vertex = predecessors[next_vertex];
			}
		}

		typename graph_traits<CSR_Graph>::out_edge_iterator it, it_end;

		//this is basically dijkstra with pushes instead of decreasekey operations
		for (tie(it,it_end) = out_edges(*(vertices(g).first+min_idx),g); it != it_end; ++it) {
			weight_type tmp = min_key + weights[*it];
			int target_index = index[target(*it, g)];

			//we only consider vertices which have not been added to the tree yet
			if (tree_predecessors[target_index] == -1 && tmp < distances[target_index]) {
				distances[target_index] = tmp;
				predecessors[target_index] = min_idx;
				heap.push(heap_data(target_index, tmp));
			}
		}
	}

	return tree_cost;
}


/* note: this method only works if tree_predecessors actually represents a tree.
 * If not sure, use function testTree(...)
 */
string SteinerTreeHeuristic::print_tree(const CSR_Graph& g, int num_vertices, int root,
		vector<int>& tree_predecessors, vector<int>& terminals) {

	std::stringstream stream;
	vector<bool> visited(num_vertices, false);

	//we iterate through the tree going from each terminal until we arrive at root
	for (unsigned int i = 0; i < terminals.size(); ++i) {
		int curr = terminals[i];
		int edge_count = 0;
		while (curr != root && !visited[curr]) {
			++edge_count;

			//every 50 lines a linebreak is added for the sake of readability
			if (edge_count % 50 == 0) stream << std::endl;
			stream << "(" << curr << "," << tree_predecessors[curr] << ") ";

			visited[curr] = true;
			curr = tree_predecessors[curr];
		}
	}

	return stream.str();
}



bool SteinerTreeHeuristic::test_tree(const CSR_Graph& g, int num_vertices, int root,
	vector<int>& tree_predecessors, vector<int>& terminals) {

	vector<bool> visited(num_vertices, false);

	//we iterate through the tree going from each terminal until we arrive at
	// the root or a vertex that we already visited
	for (unsigned int i = 0; i < terminals.size(); ++i) {
		int curr = terminals[i];
		int vertex_count = 1;

		while (curr != root && !visited[curr] && vertex_count != num_vertices) {
			//if one terminal is not connected to root, result is false
			if (curr == -1) return false;
			visited[curr] = true;
			curr = tree_predecessors[curr];
			++vertex_count;
		}

		//if a cycle is found, result is false
		if (vertex_count == num_vertices || curr == -1){
			return false;
		}
	}

	return true;
}



