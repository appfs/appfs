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
 * This method uses a self-implemented binary heap data structure for the prioritiy list, and a self-implemented dijkstra algorithm.<br>
 * 
 * * compile: use `make`<br>
 * * run: `./ex7 filepath/graph.gph [source node]`<br>
 */

#include "utils.h"
#include "heap.h"
#include "dijkstra_alg.h"

int main(int argc, char*argv[]){
  
  if (argc < 3){ // must have filename of graph and a flag of some sort...
    std::cerr << "Missing graph file or source node." << std::endl;
    return -1;
  }
  
  std::ifstream file(argv[1]);  // read graph file.
  int startNode = std::atoi(argv[2]);
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
  
  std::vector<Edge> subgraphEdges; /// unnecessary for dijkstra method, but needed for steiner graph.
  
  std::pair<std::vector<Edge>,std::vector<int>> edgesWeights = utils::get_EdgesWeights(n, file);
  
  std::cout << "building adjacency list..." << std::endl;
  std::vector<std::vector<std::pair<int,int>>> adjList = utils::build_adjList(n, edgesWeights.first, edgesWeights.second);
  
  boost::timer::cpu_timer timer;
  std::cout << "populating priority queue..." << std::endl;
  //boost::timer::cpu_times times0 = timer.elapsed();
  std::cout << "calculating distances..." << std::endl;
  f = dijkstra_alg::alg(n, adjList, startNode, subgraphEdges);
  boost::timer::cpu_times times = timer.elapsed();
  /* end dijkstra algorithm according to flag */
  
  /// output vertex and distance of the longest-shortest path.
  std::cout << "RESULT VERTEX " << f.first << std::endl;
  std::cout << "RESULT DIST " << f.second <<  std::endl;
  std::cout << std::endl;
  
    /// print CPU- and Wall-Time. 
  // boost::timer::cpu_times returns tuple of wall, system and user times in nanoseconds.
//   std::cout << "WALL-CLOCK " << times.wall / 1e9 << "s" << std::endl;
//   std::cout << "USER TIME " << times.user / 1e9 << "s" << std::endl;

  
  file.close();  
  return 0;
}
