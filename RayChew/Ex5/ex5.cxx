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
#include <boost/bimap.hpp>
#include <boost/bimap/multiset_of.hpp>
#include <boost/bimap/support/lambda.hpp>
#include <boost/bind.hpp>

namespace bm = boost::bimaps;
using namespace boost::spirit;
using qi::int_;
using qi::double_;
using qi::parse;

std::pair<int,int> m1 (int& n, std::ifstream& file){
    /* start get list of edges and weights */
  typedef std::pair<int, int> Edge;
  std::vector<Edge> Edges; // vector to store std::pair of edges.
  std::vector<int> Weights; // vector to weights as integers.
  
  std::string str;
  
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
      maxDistance = distances[*vertexPair.first];
    }
  }
    std::pair<int,int> Final = std::make_pair(maxVertex,maxDistance);
    return Final;
}

std::pair<int,int> m2 (int& n, std::ifstream& file){
  /* start get list of edges and weights */
  using vertex =  std::pair<int, int>;
  std::vector<std::vector<vertex>> adjList(n);
  
  std::cout << "pass initialize adjList." << std::endl;
  std::string str;
  
  while (getline(file,str)){ // get graph line-by-line.
    int Vert1;
    int Vert2;
    int Weight; // are all weights integer-valued ??
    
    auto it = str.begin(); // initialize iterator for qi::parse. 
    
    parse(it, str.end(), int_[([&Vert1](int i){Vert1 = i;})] >> qi::space >> int_[([&Vert2](int i){Vert2 = i;})] >> qi::space >> double_[([&Weight](double i){Weight = i;})]);  
    
    vertex VertexWeight1(Vert2,Weight);
    adjList[Vert1].push_back(VertexWeight1);
    
    vertex VertexWeight2(Vert1,Weight);
    adjList[Vert2].push_back(VertexWeight2);
  }
  /* end get list of edges and weights */
  
  std::cout << "pass build adjList." << std::endl;
  
  using bimap = bm::bimap<int, boost::bimaps::multiset_of<int,std::less<int>>>;

  bimap Unvisited;
  std::vector<vertex> finalWeights(n);
  
  for(int i=1; i<n; i++){
    Unvisited.left.insert(bimap::left_value_type(i,std::numeric_limits<int>::max()));
    finalWeights[i] = std::make_pair(i, std::numeric_limits<int>::max());
  }
  bimap::right_iterator itr = Unvisited.right.begin();
  Unvisited.right.replace_key(itr, 0);
  finalWeights[1].second = 0;

  std::cout << "pass initialize unvisited set." << std::endl;
  
  while(Unvisited.size()>0){
    auto minPair = Unvisited.right.begin();
    int minIdx = minPair->second;
    int minDist = minPair->first;
    
    for(int j=0; j<adjList[minIdx].size(); j++){
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
    
    std::sort(finalWeights.begin(), finalWeights.end(), [](auto &left, auto &right) {
      return left.second < right.second;});
    
    int maxDistance = 0;
    int maxVertex = 0;
    
    for(auto ita = finalWeights.begin(); ita != finalWeights.end(); ita++){
      if ((ita->second > maxDistance) && (ita->second < std::numeric_limits<int>::max())){
      maxDistance = ita->second;
      maxVertex = ita->first;
    }
    // if distance == maxDistance, check if vertex index is smaller.
    if ((ita->second == maxDistance) && (ita->first < maxVertex)){
      maxVertex = ita->first;
    }
    }
    vertex Final = std::make_pair(maxVertex,maxDistance);
    return Final;
}

int main(int argc, char*argv[]){
  
  if (argc < 2)
  {
    std::cerr << "No file!!!" << std::endl;
    return -1;
  }
  
  std::ifstream file(argv[1]); // std::ifstream::in ??
  std::string str;
  

  /* start get number of edges */
  getline(file, str);
  
  int n;

  auto it = str.begin();
  bool r = parse(it, str.end(),
		 int_[([&n](int i){n = i;})] >> int_);
  n = n + 1;
  /* end get number of edges */
  
  std::pair<int,int> f;
  boost::timer::cpu_timer timer;
  for(int i = 0; i < argc; i++){
    if (std::string(argv[i]) == "-m1"){
      f = m1(n,file);
        
    }
    else if(std::string(argv[i]) == "-m2"){
      f = m2(n,file);
        
    }
   else{f=std::make_pair(0,0);}
  }
  boost::timer::cpu_times times = timer.elapsed();
  
  // output vertex and distance of the longest-shortest path.
  std::cout << "RESULT VERTEX " << f.first << std::endl;
  std::cout << "RESULT DIST " << f.second <<  std::endl;
  /* end find longest-shortest path */

  // print CPU- and Wall-Time. 
  // boost::timer::cpu_times returns tuple of wall, system and user times in nanoseconds.
  std::cout << std::endl;
  std::cout << "WALL-CLOCK " << times.wall / 1e9 << "s" << std::endl;
  std::cout << "USER TIME " << times.user / 1e9 << "s" << std::endl;
  
  file.close();  
  return 0;
}
