//////////////////////////////////
//
// Compile: g++ -std=c++14 -O3 ex5.cxx -o ex5 -lboost_timer
//
//////////////////////////////////

#include <iostream>		// std::cout
#include <fstream>		// std::ifsteam
#include <utility>		// std::pair
#include <vector>		// std::vector
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp> 
#include <boost/spirit/include/qi.hpp>
#include <boost/timer/timer.hpp>

int main(int argc, char*argv[]){
  
  if (argc != 2)
  {
    std::cerr << "No file!!!" << std::endl;
    return -1;
  }
  
  std::ifstream file(argv[1]); // std::ifstream::in ??
  std::string str;
  
  boost::timer::cpu_timer timer;

  /* start get number of edges */
  getline(file, str);

  using namespace boost::spirit;
  using qi::int_;
  using qi::double_;
  using qi::parse;
  
  int n;

  auto it = str.begin();
  bool r = parse(it, str.end(),
		 int_[([&n](int i){n = i;})] >> int_);  
  /* end get number of edges */
  
  
  /* start get list of edges and weights */
  typedef std::pair<int, int> Edge;
  std::vector<Edge> Edges; // vector to store std::pair of edges.
  std::vector<int> Weights; // vector to weights as integers.
  
  while (getline(file,str)){ // get graph line-by-line.
    int Vert1;
    int Vert2;
    double Weight; // are all weights integer-valued ??
    
    auto it = str.begin(); // initialize iterator for qi::parse. 
    
    bool r = parse(it, str.end(), // parse graph.
		 int_[([&Vert1](int i){Vert1 = i;})] >> qi::space >> int_[([&Vert2](int i){Vert2 = i;})] >> qi::space >> double_[([&Weight](double i){Weight = i;})]);  
    
    Edge edge = std::make_pair(Vert1, Vert2); // make edge-pair out of vertices.
    Edges.push_back(edge);
    Weights.push_back(Weight);
  }
  /* end get list of edges and weights */
  
  
  typedef boost::property<boost::edge_weight_t, double> EdgeWeightProperty; // initialize type to store weights on edges.
  // adjacency_list<out-edges, vertex_set, directedness, vertex properties, edge properties>
  typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::no_property, EdgeWeightProperty> Graph; // create graph.

  Graph g(Edges.begin(),Edges.end(), Weights.begin(), n); // populate graph.
 
  
  // here, the graph should be built... Find shortest path.
  typedef boost::graph_traits<Graph>::vertex_descriptor vertex_descriptor;
  vertex_descriptor source = boost::vertex(1, g); // define source vertex as vertex with index == 1.
  std::vector<vertex_descriptor> parents(boost::num_vertices(g)); // initialize vectors for predecessor and distances.
  std::vector<int> distances(boost::num_vertices(g));
  
  
  // find shortest distances.
  boost::dijkstra_shortest_paths(g, source, boost::predecessor_map(&parents[0]).distance_map(&distances[0]));

  /* start find longest-shortest path */
  int maxDistance = 0;
  int maxVertex = 0;
  
  // create iterator over vertices.
  // vertexPair.first is the iterated element, and .second is the end-index of all vertices.
  typedef boost::graph_traits <Graph>::vertex_iterator vertex_iter;
  std::pair<vertex_iter, vertex_iter> vertexPair;
  
  for (vertexPair = boost::vertices(g); vertexPair.first != vertexPair.second; ++vertexPair.first) // vertexPair = boost::vertices loops over all vertices in g.
  {
    // replace maxDistance if a greater distance is found, and maxDistance must be less than "infinity" (of 32-bit signed integer).
    if ((distances[*vertexPair.first] > maxDistance) && (distances[*vertexPair.first] < 2147483647)){
      maxDistance = distances[*vertexPair.first];
      maxVertex = *vertexPair.first;
    }
    // if distance == maxDistance, check if vertex index is smaller.
    if ((distances[*vertexPair.first] == maxDistance) && (*vertexPair.first < maxVertex)){
      maxVertex = *vertexPair.first;
    }
  }
  
  boost::timer::cpu_times times = timer.elapsed();
  
  // output vertex and distance of the longest-shortest path.
  std::cout << "RESULT VERTEX " << maxVertex << std::endl;
  std::cout << "RESULT DIST " << maxDistance <<  std::endl;
  /* end find longest-shortest path */

  // print CPU- and Wall-Time. 
  // boost::timer::cpu_times returns tuple of wall, system and user times in nanoseconds.
  std::cout << std::endl;
  std::cout << "WALL-CLOCK " << times.wall / 1e9 << "s" << std::endl;
  std::cout << "USER TIME " << times.user / 1e9 << "s" << std::endl;
  
  file.close();  
  return 0;
}
