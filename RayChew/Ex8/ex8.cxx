/**
 * \file ex8.cxx
 * \author Ray Chew
 * \date 13 July 2017
 * \brief Steiner-Tree builder for undirected .gph graphs
 */

/**
 * \mainpage Ex8
 * 
 * \section Description
 * 
 * * compile: `g++ -std=c++14 utils.cxx checker.cxx boost_dijkstra.cxx m2alg.cxx ex8.cxx -o ex8 -lboost_timer -Wall`<br>
 * * run: `./ex8 filepath/graph.gph [-m1/-m2] [source node]`<br>
 * * flags: `-m1` for boost dijkstra algorithm for shortest path, `-m2` for self-implemented algorithm.
 */

#include "utils.h"
#include "checker.h"
#include "boost_dijkstra.h"
#include "m2alg.h"

int main(int argc, char* argv[]) {
  
  int startTerminal = std::atoi(argv[3]);
  
  if (argc < 3) { // must have filename of graph and a flag of some sort...
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
  n++;
  /* end get number of edges */
  
  std::vector<int> primesAll = utils::gen_Primes(n);
  std::pair<std::vector<Edge>,std::vector<int>> edgesWeights = utils::get_EdgesWeights(n, file);
  
  /* temporary stuff, if not all primes are present in graph */
  std::vector<int> nodesInGraph = utils::get_Nodes(edgesWeights.first);
  std::vector<int> restrictedPrimes;
  
  std::set_intersection(nodesInGraph.begin(),nodesInGraph.end(),primesAll.begin(),primesAll.end(),std::back_inserter(restrictedPrimes));
  std::vector<int> primes = restrictedPrimes;
  std::cout << "number of terminals in graph: " << primes.size() << std::endl;
  std::cout << std::endl;
  /* end temporary stuff */
  
  std::vector<std::pair<int,int>> subgraphEdges;
  std::vector<int> subgraphWeights;
  std::vector<int> dists;
  
  int objVal = 0;
  int currentPrime = startTerminal;
  
  /* start dijkstra algorithm according to flag */
  boost::timer::cpu_timer timer;
  for(int i = 0; i < argc; i++) {
    if (std::string(argv[i]) == "-m1") {
      
      std::cout << "building graph..." << std::endl;
      std::vector<vertex_descriptor> parents;
      Graph g = boost_dijkstra::build_Graph(n, edgesWeights.first, edgesWeights.second);
      
      std::cout << "calculating steiner-tree..." << std::endl;
      
      for(auto j = 0lu, end = primes.size(); j<end; j++) { // Assumption: All prime nodes (terminals) are connected.

	if (j%500==0) {
	  if (j+500-1 > primes.size()) {
	    std::cout << "processing terminals number: " << j << " to " << primes.size() << std::endl;
	  }
	  else {
	    std::cout << "processing terminals number: " << j << " to " << j + (500-1) << std::endl;
	  }
	}
	
	std::pair<std::vector<int>,std::vector<vertex_descriptor>> shortestDists = boost_dijkstra::get_ShortestDist(g,currentPrime);
	
	dists = shortestDists.first;
	parents = shortestDists.second;
	
	std::vector<int> primeDists = utils::get_PrimeDists(dists, primes);
	std::pair<int,int> minPrimeIdxDist = utils::get_Min(primeDists,primes);
	
	int minPrimeIdx = minPrimeIdxDist.first;
	int minPrime = primes[minPrimeIdx];
	objVal += minPrimeIdxDist.second;
	
	int currentWeight = std::numeric_limits<int>::max();
	int currentNode = minPrime;
	int pred = parents[currentNode];
	
	while(currentWeight != 0) {
	  Edge edge = std::make_pair(pred,currentNode);
	  boost::graph_traits<Graph>::vertex_descriptor predV;
	  boost::graph_traits<Graph>::vertex_descriptor currentNodeV;
	  predV = boost::vertex(pred,g);
	  currentNodeV = boost::vertex(currentNode,g);
	  bool found;
	  boost::graph_traits<Graph>::edge_descriptor ed;
	  boost::tie(ed, found) = boost::edge(predV, currentNodeV, g);
	  
	  boost::property_map<Graph, boost::edge_weight_t>::type weight = get(boost::edge_weight, g);
	  currentWeight = get(weight, ed);
	  if (currentWeight == 0) {
	    break;
	  }
	  subgraphEdges.push_back(edge);
	  subgraphWeights.push_back(currentWeight);
	  boost::put(boost::edge_weight, g, ed, 0);
	  
	  currentNode = pred;
	  pred = parents[currentNode];
	  if (pred == currentNode) {
	    break;
	  }
	}
	currentPrime = minPrime;
      }
      std::cout << "objVal = " << objVal << " edge count = " << subgraphEdges.size() << std::endl;
      std::cout << std::endl;
      
      std::cout << "checking steiner tree..." << std::endl;
      checker::isConnected(subgraphEdges, startTerminal);
      checker::isFeasible(subgraphEdges,primes);
    }
    
    else if(std::string(argv[i]) == "-m2") {
      
      std::cout << "building graph..." << std::endl;
      std::vector<int> parents;
      std::vector<std::vector<std::pair<int,int>>> adjList = utils::build_adjList(n, edgesWeights.first, edgesWeights.second);
      
      std::cout << "calculating steiner-tree..." << std::endl;
      for(auto j = 0lu, end=primes.size(); j<end; j++) {
	
	if (j%500==0){
	  if (j+500-1 > primes.size()){
	    std::cout << "processing terminals number: " << j << " to " << primes.size() << std::endl;
	  }
	  else{
	    std::cout << "processing terminals number: " << j << " to " << j + (500-1) << std::endl;
	  }
	}
	
	bimap Unvisited = m2alg::build_unvisited(n, currentPrime, subgraphEdges);
	
	std::pair<std::vector<int>,std::vector<int>> shortestDists = m2alg::m2Algorithm(n, Unvisited, adjList, currentPrime);
	dists = shortestDists.first;
	parents = shortestDists.second;
	
	std::vector<int> primeDists = utils::get_PrimeDists(dists, primes);
	std::pair<int,int> minPrimeIdxDist = utils::get_Min(primeDists,primes);
	
	int minPrimeIdx = minPrimeIdxDist.first;
	int minPrime = primes[minPrimeIdx];
	objVal += minPrimeIdxDist.second;
	
	int currentWeight = std::numeric_limits<int>::max();
	int currentNode = minPrime;
	int pred = parents[currentNode];
	
	while(currentWeight != 0) {
	  //std::cout << "currentNode: " << currentNode << " pred: " << pred << std::endl;
	  Edge edge = std::make_pair(pred,currentNode);

	  int predIdx;
	  int currentNodeIdx;
	  
	  predIdx = m2alg::get_NeighbourIndex(currentNode, pred, adjList);
	  currentWeight = adjList[currentNode][predIdx].second;
	  
	  if (currentWeight == 0) {
	    break;
	  }
	  
	  subgraphEdges.push_back(edge);
	  subgraphWeights.push_back(currentWeight);
	  
	  // change edge weight on both directions (undirected graph)
	  adjList[currentNode][predIdx].second = 0;
	  currentNodeIdx = m2alg::get_NeighbourIndex(pred, currentNode,adjList);
	  adjList[pred][currentNodeIdx].second = 0;
	  
	  currentNode = pred;
	  pred = parents[currentNode];
	  if (pred == currentNode) {
	    break;
	  }
	}
	currentPrime = minPrime;
      }
      std::cout << "objVal = " << objVal << " edge count = " << subgraphEdges.size() << std::endl;
      std::cout << std::endl;
      
      std::cout << "checking steiner tree..." << std::endl;
      checker::isConnected(subgraphEdges, startTerminal);
      checker::isFeasible(subgraphEdges,primes);
    }
  }
  boost::timer::cpu_times times = timer.elapsed();
  
  /// print CPU- and Wall-Time. 
  std::cout << std::endl;
  std::cout << "WALL-CLOCK " << times.wall / 1e9 << "s" << std::endl;
  std::cout << "USER TIME " << times.user / 1e9 << "s" << std::endl;
  std::cout << std::endl << std::endl;
  
  file.close();  
  return 0;
}
