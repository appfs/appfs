#include "boost_dijkstra.h"

Graph boost_dijkstra::build_Graph(int& n, std::vector<Edge>& Edges, std::vector<int>& Weights) {
  /* start building graph */
  Graph g(Edges.begin(),Edges.end(), Weights.begin(), n);   /// populate graph.
  /* end building graph */
  return g;
}

std::pair<std::vector<int>,std::vector<vertex_descriptor>> boost_dijkstra::get_ShortestDist(Graph& g, int& sourceVertex) {
  /* start finding shortest path from source node. */
  vertex_descriptor source = boost::vertex(sourceVertex, g);
  /// initialize vectors for predecessor and distances.
  std::vector<vertex_descriptor> parents(boost::num_vertices(g));
  std::vector<int> distances(boost::num_vertices(g));
  
  boost::dijkstra_shortest_paths(g, source, boost::predecessor_map(&parents[0]).distance_map(&distances[0]));
  /* end finding shortest path of from source node. */
  return std::make_pair(distances,parents);
} 
