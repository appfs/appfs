/**
 * \file ex7.cxx
 * \author Ray Chew
 * \date 7 July 2017
 * \brief Dijkstra graph algorithm for .gph graphs
 */

/**
 * \mainpage Ex7
 * 
 * \section Description
 * 
 * Shortest-longest path distance calculator using the Dijkstra algorithm for `.gph` graphs.<br>
 * Reads `.gph` graph file and prints the node number and distance of the node corresponding to the end of longest shortest path.<br>
 * Flag `-m1` uses the Boost Graph Library Dijkstra Algorithm.<br>
 * Flag `-m2` uses a self-implemented Dijkstra Algorithm for undirected graphs.<br>
 * 
 * * compile: `g++ -std=c++14 -O3 ex7.cxx -o ex7 -lboost_timer -Wall`<br>
 * * run: `./ex7 filepath/graph.gph [-m1/-m2]`<br>
 * * flags: `-m1` for boost algorithm, `-m2` for self-implemented algorithm.
 */

/* -- Includes -- */
/* C++ includes. */
#include <iostream> /* for std::cout, std::ofstream */
#include <fstream> /* for fstream::app */
#include <utility> /* for std::pair */
#include <vector> /* for std::vector */

/* Boost Dijkstra Algorithm includes. */
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp> 

/* Boost qi parser and timer includes. */
#include <boost/spirit/include/qi.hpp>
#include <boost/timer/timer.hpp>

/* Boost Bimap Container includes for own algorithm. */
#include <boost/bimap.hpp>
#include <boost/bimap/multiset_of.hpp>
#include <boost/bimap/support/lambda.hpp> /* for bimap::_data and bimap::_key */
//#include <boost/bind.hpp> /* for boost::bind in sorting of vector<pair<int,int>> */

namespace bm = boost::bimaps;
using namespace boost::spirit;
using qi::int_;
using qi::double_;
using qi::parse;

/** 
 *  \brief A method that calculates the longest-shortest path from the source node with name `1` using the boost library dijkstra algorithm.
 *  \param n number of edges as int.
 *  \param file pointer to graph file opened.
 *  \return MaxVertex and MaxDistance, the node name and distance of the longest-shortest path as `std::pair<int,int>`.
 */
std::pair<int,int> m1 (int& n, std::ifstream& file){
  
  /* start get list of edges and weights */
  using Edge = std::pair<int, int>; 
  std::vector<Edge> Edges; // vector to store std::pair of edges.
  std::vector<int> Weights; // vector to weights as integers.
  std::string str; // string to store line of graph file.
  
  while (getline(file,str)){ /// get graph line-by-line.
    int Vert1;
    int Vert2;
    int Weight;
    
    auto it = str.begin(); /// initializes iterator for qi::parse. 
    
    parse(it, str.end(), int_[([&Vert1](int i){Vert1 = i;})] >> qi::space >> int_[([&Vert2](int i){Vert2 = i;})] >> qi::space >> double_[([&Weight](int i){Weight = i;})]);  
    
    Edge edge = std::make_pair(Vert1, Vert2);  /// make edge-pair out of vertices.
    Edges.push_back(edge);
    Weights.push_back(Weight);
  }
  /* end get list of edges and weights */
  
  /* start building graph */
  /// initialize type to store weights on edges.
  typedef boost::property<boost::edge_weight_t, int> EdgeWeightProperty;
  // adjacency_list<out-edges, vertex_set, directedness, vertex properties, edge properties>
  /// create graph.
  typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::no_property, EdgeWeightProperty> Graph;
  
  Graph g(Edges.begin(),Edges.end(), Weights.begin(), n);   /// populate graph.
  
  /* end building graph */
  
  
  /* start finding shortest path from source node. */
  typedef boost::graph_traits<Graph>::vertex_descriptor vertex_descriptor;
  vertex_descriptor source = boost::vertex(1, g);   /// define source vertex as vertex with index == 1.
  /// initialize vectors for predecessor and distances.
  std::vector<vertex_descriptor> parents(boost::num_vertices(g));
  std::vector<int> distances(boost::num_vertices(g));
  
  boost::dijkstra_shortest_paths(g, source, boost::predecessor_map(&parents[0]).distance_map(&distances[0]));
  
  /* end finding shortest path of from source node. */
  
  
  /* start finding longest-shortest path from source node. */
  signed int maxDistance = 0;
  unsigned int maxVertex = 0;
  
  /// create iterator over vertices.
  // vertexPair.first is the iterated element, and .second is the end-index of all vertices.
  typedef boost::graph_traits <Graph>::vertex_iterator vertex_iter;
  std::pair<vertex_iter, vertex_iter> vertexPair;
  
  // vertexPair = boost::vertices loops over all vertices in g.
  for (vertexPair = boost::vertices(g); vertexPair.first != vertexPair.second; ++vertexPair.first){
    /// replace maxDistance if a greater distance is found, and maxDistance must be less than "infinity" (of 32-bit signed integer).
    if ((distances[*vertexPair.first] > maxDistance) && (distances[*vertexPair.first] < std::numeric_limits<int>::max())){
      maxDistance = distances[*vertexPair.first];
      maxVertex = *vertexPair.first;
    }
    /// if distance == maxDistance, check if vertex index is smaller.
    if ((distances[*vertexPair.first] == maxDistance) && (*vertexPair.first < maxVertex)){
      maxDistance = distances[*vertexPair.first];
    }
  }
  /* end finding longest-shortest path from source node. */

  std::pair<int,int> Final = std::make_pair(maxVertex,maxDistance);
  return Final;
}


/** 
 *  \brief A method that calculates the longest-shortest path from the source node with name `1` using a self-implemented dijkstra algorithm.
 *  \param n number of edges as int.
 *  \param file pointer to graph file opened.
 *  \return MaxVertex and MaxDistance, the node name and distance of the longest-shortest path as `std::pair<int,int>`.
 */
std::pair<int,int> m2 (int& n, std::ifstream& file){
  
  /* start populating adjacency list */
  using vertex =  std::pair<int, int>;
  std::vector<std::vector<vertex>> adjList(n); // empty adjacency list.
  
  std::cout << "pass initialize adjList." << std::endl;
  std::string str; // string to store line of graph.
  
  while (getline(file,str)){
    int Vert1;
    int Vert2;
    int Weight;
    
    auto it = str.begin(); /// initialize iterator for qi::parse. 
    
    parse(it, str.end(), int_[([&Vert1](int i){Vert1 = i;})] >> qi::space >> int_[([&Vert2](int i){Vert2 = i;})] >> qi::space >> double_[([&Weight](double i){Weight = i;})]);  
    
    vertex VertexWeight1(Vert2,Weight);
    adjList[Vert1].push_back(VertexWeight1);
    
    vertex VertexWeight2(Vert1,Weight);
    adjList[Vert2].push_back(VertexWeight2);
  }
  std::cout << "pass build adjList." << std::endl;
  /* end populating adjacency list */
  
  
  using bimap = bm::bimap<int, boost::bimaps::multiset_of<int,std::less<int>>>;
  bimap Unvisited; /// define unvisited set as a boost::bimap container.
  std::vector<vertex> finalWeights(n); // vector to store all calculated weights/distances.
  
  /* start initializing unvisited set */
  for(int i=1; i<n; i++){ 
    Unvisited.left.insert(bimap::left_value_type(i,std::numeric_limits<int>::max()));
    finalWeights[i] = std::make_pair(i, std::numeric_limits<int>::max());
  }
  bimap::right_iterator itr = Unvisited.right.begin();
  Unvisited.right.replace_key(itr, 0);
  finalWeights[1].second = 0;
  
  std::cout << "pass initialize unvisited set." << std::endl;
  /* end initializing unvisited set */
  
  
  /* start calculating and updating distances */
  while(Unvisited.size()>0){
    auto minPair = Unvisited.right.begin();
    int minIdx = minPair->second;
    int minDist = minPair->first;
    
    signed int adjListSize = adjList[minIdx].size();
    
    for(int j=0; j<adjListSize; j++){
      int neighbour = adjList[minIdx][j].first;
      int dist = adjList[minIdx][j].second;
      int newDist = minDist + dist;
      int nPWeight = finalWeights[neighbour].second;
      
      if (newDist < nPWeight){
	auto toBeReplaced = Unvisited.left.find(neighbour);
	Unvisited.left.modify_data(toBeReplaced, bm::_data=newDist);
	finalWeights[neighbour].second = newDist;
      }
    }
    Unvisited.left.erase(minIdx);
  }
  std::cout << "pass distance calculation." << std::endl;
  /* end calculating and updating distances */
  
  /* start find node of the longest-shortest path */
  //std::sort(finalWeights.begin(), finalWeights.end(), [](auto &left, auto &right) {
  //    return left.second < right.second;}); 
  
  int maxDistance = 0;
  int maxVertex = 0;
  
  for(auto ita = finalWeights.begin(); ita != finalWeights.end(); ita++){
    /// replace maxDistance if a greater distance is found, and maxDistance must be less than "infinity" (of 32-bit signed integer).
    if ((ita->second > maxDistance) && (ita->second < std::numeric_limits<int>::max())){
      maxDistance = ita->second;
      maxVertex = ita->first;
    }
    /// if distance == maxDistance, check if vertex index is smaller.
    if ((ita->second == maxDistance) && (ita->first < maxVertex)){
      maxVertex = ita->first;
    }
  }
  /* end find node of the longest-shortest path */
  
  vertex Final = std::make_pair(maxVertex,maxDistance);
  return Final;
}


int main(int argc, char*argv[]){
  
  if (argc < 3){ // must have filename of graph and a flag of some sort...
    std::cerr << "No file or flag (-m1 or -m2)!!!" << std::endl;
    return -1;
  }
  
  std::ifstream file(argv[1]);  // read graph file.
  std::string str; /// read graph file line by line.
  
  
  /* start get number of edges */
  getline(file, str);
  int n; /// store n as int for number of edges.
  
  auto it = str.begin();
  parse(it, str.end(), int_[([&n](int i){n = i;})] >> int_);
  n = n + 1; // No?
  /* end get number of edges */
  
  
  /* start dijkstra algorithm according to flag */
  std::pair<int,int> f =std::make_pair(0,0); /// initialize f as (int,int) pair for final node number and distance.
  boost::timer::cpu_timer timer;
  for(int i = 0; i < argc; i++){
    if (std::string(argv[i]) == "-m1"){
      f = m1(n,file);
    }
    else if(std::string(argv[i]) == "-m2"){
      f = m2(n,file);
    }
  }
  boost::timer::cpu_times times = timer.elapsed();
  /* end dijkstra algorithm according to flag */
  
  
  /// output vertex and distance of the longest-shortest path.
  std::cout << "RESULT VERTEX " << f.first << std::endl;
  std::cout << "RESULT DIST " << f.second <<  std::endl;
  
  /// print CPU- and Wall-Time. 
  // boost::timer::cpu_times returns tuple of wall, system and user times in nanoseconds.
  std::cout << std::endl;
  std::cout << "WALL-CLOCK " << times.wall / 1e9 << "s" << std::endl;
  std::cout << "USER TIME " << times.user / 1e9 << "s" << std::endl;
  
  file.close();  
  return 0;
}
