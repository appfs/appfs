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
		int root, vector<int>& tree_preds, const vector<int>& terminals) {

	assert((int) tree_preds.size() == num_vertices);

	// terminals are scanned
	int num_terminals = terminals.size();
	vector<bool> is_terminal(num_vertices, false);
	for (int i = 0; i < num_terminals; ++i) {
		assert(terminals[i] < num_vertices);
		is_terminal[terminals[i]] = true;
	}

	assert(is_terminal[root]);

	heap::d_ary_heap<heap_data, heap::arity<4>> heap;

	// this vector is used to store the info about predecessor and the respective
	// edge together in order to avoid cache misses
	vector<pair<int,weight_type>> preds(num_vertices);
	preds[root] = std::make_pair(root, 0);

	// we expect the tree vector to be filled with -1 already
	for (int i = 0; i < num_vertices; ++i) {
		assert(tree_preds[i] == -1);
	}

	// initialization of distances. all but the root vertex have infinity
	vector<weight_type> dist(num_vertices, std::numeric_limits<weight_type>::infinity());
	dist[root] = 0;
	tree_preds[root] = root;

	heap.push(heap_data(root, 0));


	// this is just boost syntax so that we can access weights and indices later
	property_map<CSR_Graph, edge_weight_t>::const_type weights = get(edge_weight, g);
	property_map<CSR_Graph, vertex_index_t>::const_type index = get(vertex_index, g);

	int connected_terminals = 1;
	weight_type tree_cost = 0;

	// here the actual algorithm starts
	while (connected_terminals < num_terminals) {

		assert(!heap.empty());	//heap should never be empty before tree contains all terminals

		int min_idx = heap.top().index;
		weight_type min_key = heap.top().key;

		heap.pop();

		if (dist[min_idx] < min_key) continue;

		// if we scan a terminal, all vertices on shortest path to subtree
		// are added to heap with weight 0 and included into the tree
		if (is_terminal[min_idx] && tree_preds[min_idx] == -1) {

			++connected_terminals;

			min_key = 0;
			int next_vertex = preds[min_idx].first;
			tree_preds[min_idx] = next_vertex;

			tree_cost += preds[min_idx].second;

			// here all vertices on path from new terminal to tree are added again with key=0
			while (tree_preds[next_vertex] == -1) {

				heap.push(heap_data(next_vertex, 0));
				dist[next_vertex] = 0;
				tree_preds[next_vertex] = preds[next_vertex].first;

				tree_cost += preds[next_vertex].second;

				next_vertex = preds[next_vertex].first;
			}
		}

		typename graph_traits<CSR_Graph>::out_edge_iterator it, it_end;

		// this is basically dijkstra with pushes instead of decreasekey operations
		for (tie(it,it_end) = out_edges(*(vertices(g).first+min_idx),g); it != it_end; ++it) {

			int v_target = index[target(*it, g)];

			// we only consider vertices which have not been added to the tree yet
			if (tree_preds[v_target] != -1) continue;

			if (min_key + weights[*it] < dist[v_target]) {
				// distance is updated
				dist[v_target] = min_key + weights[*it];
				preds[v_target].first = min_idx;
				preds[v_target].second = weights[*it];
				heap.push(heap_data(v_target, dist[v_target]));
			}
		}
	}

	return tree_cost;
}


/* note: this method only works if tree_preds actually represents a tree.
 * If not sure, use function testTree(...)
 */
string SteinerTreeHeuristic::print_tree(const CSR_Graph& g, int num_vertices, int root,
		vector<int>& tree_preds, vector<int>& terminals) {

	std::stringstream stream;
	vector<bool> visited(num_vertices, false);

	// we iterate through the tree going from each terminal until we arrive at root
	for (unsigned int i = 0; i < terminals.size(); ++i) {
		int curr = terminals[i];
		int edge_count = 0;
		while (curr != root && !visited[curr]) {
			++edge_count;

			// every 50 lines a linebreak is added for the sake of readability
			if (edge_count % 50 == 0) stream << std::endl;
			stream << "(" << curr << "," << tree_preds[curr] << ") ";

			visited[curr] = true;
			curr = tree_preds[curr];
		}
	}

	return stream.str();
}



bool SteinerTreeHeuristic::test_tree(const CSR_Graph& g, int num_vertices, int root,
	vector<int>& tree_preds, vector<int>& terminals) {

	vector<bool> visited(num_vertices, false);

	// we iterate through the tree going from each terminal until we arrive at
	// the root or a vertex that we already visited
	for (unsigned int i = 0; i < terminals.size(); ++i) {
		int curr = terminals[i];
		int vertex_count = 1;

		while (curr != root && !visited[curr] && vertex_count != num_vertices) {
			// if one terminal is not connected to root, result is false
			if (curr == -1) return false;
			visited[curr] = true;
			curr = tree_preds[curr];
			++vertex_count;
		}

		// if a cycle is found, result is false
		if (vertex_count == num_vertices || curr == -1){
			return false;
		}
	}

	return true;
}



