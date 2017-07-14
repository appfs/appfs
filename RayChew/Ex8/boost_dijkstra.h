/* Boost Dijkstra Algorithm includes. */
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include "utils.h"

#ifndef BOOST_DIJKSTRA_H
#define BOOST_DIJKSTRA_H

using EdgeWeightProperty = boost::property<boost::edge_weight_t, int>; // initialize type to store weights on edges.
// adjacency_list<out-edges, vertex_set, directedness, vertex properties, edge properties>
using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::no_property, EdgeWeightProperty>;
using vertex_descriptor = boost::graph_traits<Graph>::vertex_descriptor;

namespace boost_dijkstra {
  Graph build_Graph(int& n, std::vector<Edge>& Edges, std::vector<int>& Weights);

  std::pair<std::vector<int>,std::vector<vertex_descriptor>> get_ShortestDist (Graph& g, int& sourceVertex);
};

#endif