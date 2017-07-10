/**
 * \file ex8.cxx
 * \author Ray Chew
 * \date 7 July 2017
 * \brief Dijkstra graph algorithm for .gph graphs
 */

/**
 * \mainpage Ex8
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
#include <iterator> /* for std::distance */
#include <queue> /* for std::queue */

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
using Edge = std::pair<int, int>; // vector to store std::pair of two vertex indices.
using vertex = std::pair<int,int>; // vector to store std::pair of vertex index and weight.
using EdgeWeightProperty = boost::property<boost::edge_weight_t, int>; // initialize type to store weights on edges.
// adjacency_list<out-edges, vertex_set, directedness, vertex properties, edge properties>
using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, boost::no_property, EdgeWeightProperty>;
using vertex_descriptor = boost::graph_traits<Graph>::vertex_descriptor;
using bimap = bm::bimap<int, boost::bimaps::multiset_of<int,std::less<int>>>;

std::vector<int> get_Nodes(std::vector<Edge>& edges){
  std::set<int> nodes;
  for (auto i = edges.begin(), end = edges.end(); i != end; i++){
    nodes.insert(i->first);
    nodes.insert(i->second);
  }
  std::vector<int> nodesAsVector;
  std::copy(nodes.begin(), nodes.end(), std::back_inserter(nodesAsVector));
  return nodesAsVector;
}

/** 
 *  \brief A method that calculates the longest-shortest path from the source node with name `1` using the boost library dijkstra algorithm.
 *  \param n number of edges as int.
 *  \param file pointer to graph file opened.
 *  \return MaxVertex and MaxDistance, the node name and distance of the longest-shortest path as `std::pair<int,int>`.
 */
std::pair<std::vector<Edge>,std::vector<int>> get_EdgesWeights (int& n, std::ifstream& file){
  /* start get list of edges and weights */
  std::cout << "getting edges and weights from file..." << std::endl;
  std::vector<Edge> Edges;
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
    if (Weight == 0){
      Weight = std::numeric_limits<int>::max();
    }
    Weights.push_back(Weight);
  }
  /* end get list of edges and weights */
  return std::make_pair(Edges,Weights);
}

Graph build_Graph (int& n, std::vector<Edge>& Edges, std::vector<int>& Weights){
  /* start building graph */
  Graph g(Edges.begin(),Edges.end(), Weights.begin(), n);   /// populate graph.
  /* end building graph */
  return g;
}

std::pair<std::vector<int>,std::vector<vertex_descriptor>> get_ShortestDist (Graph& g, int& sourceVertex){
  /* start finding shortest path from source node. */
  vertex_descriptor source = boost::vertex(sourceVertex, g);
  /// initialize vectors for predecessor and distances.
  std::vector<vertex_descriptor> parents(boost::num_vertices(g));
  std::vector<int> distances(boost::num_vertices(g));
  
  boost::dijkstra_shortest_paths(g, source, boost::predecessor_map(&parents[0]).distance_map(&distances[0]));
  /* end finding shortest path of from source node. */
  return std::make_pair(distances,parents);
}


std::vector<int> gen_Primes(int& n){ // cite: https://stackoverflow.com/questions/5200879/printing-prime-numbers-from-1-through-100/
  std::vector<int> primes;
  primes.push_back(2);
  for(int i=3; i < n; i=i+2){
    bool isPrime=true;
    for(auto j = primes.begin(), end = primes.end();j != end && (*j)*(*j) <= i;j++){
      if(i % *j == 0){
	isPrime=false;
	break;
      }
    }
    if(isPrime){
      primes.push_back(i);
    }
  }
  return primes;
}

std::vector<int> get_PrimeDists(std::vector<int>& dists, std::vector<int>& primes){
  int j = 0;
  std::vector<int> primeDists(primes.size());
  for(auto i=primes.begin(), end=primes.end(); i!=end; i++){
    primeDists[j] = dists[*i];
    j++;
    //std::cout << primes[i] << " " << primeDists[i] << std::endl;
  }
  return primeDists;
}

std::pair<int,int> get_Min(std::vector<int>& primeDists, std::vector<int>& primes){ // If two prime vertices have the minimum weight, the one with the smaller index is chosen.
  std::vector<int>::iterator minDistAddr = std::min_element(primeDists.begin(), primeDists.end(),[](const int& i, const int& j) {return i > 0 && (j<=0 || i<j);}); // minimum weight larger than zero.
  int minDistIdx = std::distance(primeDists.begin(),minDistAddr);
  //int minPrime = primes[minDistIdx];
  int minDist = *minDistAddr;
  return std::make_pair(minDistIdx, minDist);
}

bimap build_unvisited(int& n, int& startTerminal, std::vector<std::pair<int,int>>& edges){
  /* start initializing unvisited set */
  bimap Unvisited; /// define unvisited set as a boost::bimap container.
  std::vector<int> nodes = get_Nodes(edges);
  for(int i=1; i<n; i++){ 
    Unvisited.left.insert(bimap::left_value_type(i, std::numeric_limits<int>::max()));
  }
  
  bimap::left_iterator itr = Unvisited.left.find(startTerminal);
  Unvisited.left.replace_data(itr, 0);
  
  if (!nodes.empty()){
    for(auto j=nodes.begin(), end=nodes.end(); j!=end; j++){
      bimap::left_iterator itr = Unvisited.left.find(*j);
      Unvisited.left.replace_data(itr, 0);    
    }
  }

  return Unvisited;
  /* end initializing unvisited set */
}

/** 
 *  \brief A method that calculates the longest-shortest path from the source node with name `1` using a self-implemented dijkstra algorithm.
 *  \param n number of edges as int.
 *  \param file pointer to graph file opened.
 *  \return MaxVertex and MaxDistance, the node name and distance of the longest-shortest path as `std::pair<int,int>`.
 */
std::pair<std::vector<int>,std::vector<int>> m2Algorithm (int& n, bimap& Unvisited, std::vector<std::vector<std::pair<int,int>>>& adjList, int& startTerminal){
  std::vector<int> finalWeights(n, std::numeric_limits<int>::max()); // vector to store all calculated weights/distances.
  std::vector<int> finalParents(n);

  finalWeights[startTerminal] = 0;
  finalParents[startTerminal] = startTerminal;
  
  /* start calculating and updating distances */
  //std::cout << "calculating distances..." << std::endl;
  while(Unvisited.size()>0){
    auto minPair = Unvisited.right.begin();
    int minIdx = minPair->second;
    int minDist = minPair->first;
    
//     typedef bimap::left_map::const_iterator left_const_iterator;
//     
//     for( left_const_iterator left_iter = Unvisited.left.begin(), iend = Unvisited.left.end(); left_iter != iend; ++left_iter ){
//       std::cout << left_iter->first << " --> " << left_iter->second << std::endl;
//     }
    //std::cout << minIdx << std::endl;
    signed int adjListSize = adjList[minIdx].size();
    
    //std::cout << "minIdx = " << minIdx << " minDist = " << minDist << std::endl;
    for(int j=0; j<adjListSize; j++){
      //std::cout << "adjList for minIdx = " << minIdx << " neighbour is " << adjList[minIdx][j].first << " and dist is " << adjList[minIdx][j].second << std::endl;
      int neighbour = adjList[minIdx][j].first;
      int dist = adjList[minIdx][j].second;
      int newDist = minDist + dist;
      int nPWeight = finalWeights[neighbour];
      
      if (newDist < nPWeight){
	auto toBeReplaced = Unvisited.left.find(neighbour);
	if (toBeReplaced != Unvisited.left.end()){
	  Unvisited.left.modify_data(toBeReplaced, bm::_data=newDist);
	}
	finalWeights[neighbour] = newDist;
	finalParents[neighbour] = minIdx;
      }
    }
    Unvisited.left.erase(minIdx);
  }
  //std::cout << "calculating distances done" << std::endl;
  return std::make_pair(finalWeights,finalParents);
  /* end calculating and updating distances */
}

int get_NeighbourIndex(int& currentNode, int& neighbour, std::vector<std::vector<vertex>>& adjList){
  int j = 0;
  for (auto i=adjList[currentNode].begin(), end=adjList[currentNode].end(); i!=end; i++){
    if (i->first==neighbour){
      break;
    }
    j++;
  }
  return j;
}

std::vector<std::vector<vertex>> build_adjList(int& n, std::vector<Edge>& edges, std::vector<int>& weights){
  std::vector<std::vector<vertex>> adjList(n); // initialize adjList such that each row starts from index 1.
  //for (int j=0; j<adjList.size(); j++){
  //  adjList[j].push_back(std::make_pair(0,0));
  //}
  int j = 0;
  for (auto i=edges.begin(), end=edges.end(); i!=end; i++){
    int vert1 = i->first;
    int vert2 = i->second;
    
    adjList[vert1].push_back(std::make_pair(vert2,weights[j]));
    adjList[vert2].push_back(std::make_pair(vert1,weights[j]));
    j++;
  }
  return adjList;
}

std::vector<std::vector<int>> build_adjList(int& n,std::vector<Edge>& edges){
  std::vector<std::vector<int>> adjList(n);
  for (auto i=edges.begin(), end=edges.end(); i!=end; i++){
    int vert1 = i->first;
    int vert2 = i->second;
    
    adjList[vert1].push_back(vert2);
    adjList[vert2].push_back(vert1);
  }
  return adjList;
}

bool isConnected(std::vector<Edge>& edges, int& startTerminal){ // cite: https://stackoverflow.com/questions/16411038/bfs-and-dfs-on-adjacency-list
  std::cout << "starting BFS..." << std::endl;
  int n = get_Nodes(edges).back();
  n++;
  std::vector<std::vector<int>> graph = build_adjList(n, edges);
  bool visited[n] = {false};
  int startNode = startTerminal;
  visited[startNode] = true;
  std::queue<std::pair<int,int>> queue;
  queue.push(std::make_pair(0,startNode));
  
  std::pair<int,int> node;
  int neighbour;
  while(!queue.empty()){
    node = queue.front();
    queue.pop();
    std::vector<int> neighbours = graph[node.second];
    for (auto i=neighbours.begin(), end=neighbours.end(); i!=end ; i++){
      neighbour = *i;
      //std::cout << "node: " << node.second << " neighbour: " << neighbour << " parent: " << node.first << std::endl;
      if(visited[neighbour] == false){
	visited[neighbour] = true;
	queue.push(std::make_pair(node.second,neighbour));
      }
      else if(neighbour != node.first){
	std::cout << "Graph is NOT connected." << std::endl;
	return false;
	break;
      }
    }
  }
  std::cout << "Graph is connected." << std::endl;
  return true;
}

bool isFeasible(std::vector<Edge>& edges, std::vector<int>& primes){
  std::vector<int> nodes = get_Nodes(edges);
  if (std::includes(nodes.begin(), nodes.end(), primes.begin(), primes.end())){
    std::cout << "Solution is feasible." << std::endl;
    return true;
  }
  else{
    std::cout << "Solution is NOT feasible." << std::endl;
    return false;
  }
}

int main(int argc, char* argv[]){
  
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
  n++;
  /* end get number of edges */
  
  //std::vector<int> primes = gen_Primes(n);
  //for(int foo = 0; foo < primes.size(); foo++){
  //  std::cout << primes[foo] << std::endl;
  //}
  
  //     signed int maxDistance = 0;
  //   unsigned int maxVertex = 0;
  std::vector<int> primesAll = gen_Primes(n);
  std::pair<std::vector<Edge>,std::vector<int>> edgesWeights = get_EdgesWeights(n, file);
  
  /* temporary stuff, if not all primes are present in graph */
  std::vector<int> nodesInGraph = get_Nodes(edgesWeights.first);
  std::vector<int> restrictedPrimes;
  //std::cout << primesAll.size() << " " << nodesInGraph.size() << std::endl;
  
  std::set_intersection(nodesInGraph.begin(),nodesInGraph.end(),primesAll.begin(),primesAll.end(),std::back_inserter(restrictedPrimes));
  std::vector<int> primes = restrictedPrimes;
  std::cout << "number of terminals in graph: " << primes.size() << std::endl;
  std::cout << std::endl;
  /* end temporary stuff */
  
  std::vector<std::pair<int,int>> subgraphEdges;
  std::vector<int> subgraphWeights;
  std::vector<int> dists;
  
  int startTerminal = 2;
  int objVal = 0;
  int currentPrime = startTerminal;
  /* start dijkstra algorithm according to flag */
  boost::timer::cpu_timer timer;
  for(int i = 0; i < argc; i++){
    if (std::string(argv[i]) == "-m1"){
      
      std::cout << "building graph..." << std::endl;
      std::vector<vertex_descriptor> parents;
      Graph g = build_Graph(n, edgesWeights.first, edgesWeights.second);
      
      std::cout << "calculating steiner-tree..." << std::endl;
      
      for(auto j = 0lu, end = primes.size(); j<end; j++){ // Assumption: All prime nodes (terminals) are connected.
	
	if (j%500==0){
	  if (j+500-1 > primes.size()){
	    std::cout << "processing terminals number: " << j << " to " << primes.size() << std::endl;
	  }
	  else{
	    std::cout << "processing terminals number: " << j << " to " << j + (500-1) << std::endl;
	  }
	}
	
	std::pair<std::vector<int>,std::vector<vertex_descriptor>> shortestDists = get_ShortestDist(g,currentPrime);
	
	dists = shortestDists.first;
	parents = shortestDists.second;
	
	std::vector<int> primeDists = get_PrimeDists(dists, primes);
	std::pair<int,int> minPrimeIdxDist = get_Min(primeDists,primes);
	
	//std::cout << "pass get minPrimeDistIdx: " << minPrimeIdxDist.first << std::endl;
	
	int minPrimeIdx = minPrimeIdxDist.first;
	int minPrime = primes[minPrimeIdx];
	objVal += minPrimeIdxDist.second;
	
//	std::cout << " ------- " << std::endl;
// 	for(int foo = 0; foo < dists.size(); foo++){
// 	std::cout << foo << " " << dists[foo] << " " << parents[foo] << std::endl;
// 	}
	
	int currentWeight = std::numeric_limits<int>::max();
	int currentNode = minPrime;
	int pred = parents[currentNode];
	
	while(currentWeight != 0){
	  //std::cout << "current nodes: " << currentNode << " pred: " << pred << " currentweight: " << currentWeight << std::endl;
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
	  if (currentWeight == 0){
	    break;
	  }
	  subgraphEdges.push_back(edge);
	  subgraphWeights.push_back(currentWeight);
	  boost::put(boost::edge_weight, g, ed, 0);
	  //std::cout << "current pred: " << pred << " with edge weight: " << currentWeight << std::endl;
	  currentNode = pred;
	  pred = parents[currentNode];
	  if (pred == currentNode){
	    //std::cout << "pred == currentNode, next!" << std::endl;
	    break;
	  }
	  //std::cout << "next!" << std::endl;
	}
	
	currentPrime = minPrime;
      }
      std::cout << "objVal = " << objVal << " edge count = " << subgraphEdges.size() << std::endl;
      std::cout << std::endl;
      
      std::cout << "checking steiner tree..." << std::endl;
      isConnected(subgraphEdges, startTerminal);
      isFeasible(subgraphEdges,primes);
      
//       for (int i = 0; i<subgraphEdges.size(); i++){
// 	std::cout << subgraphEdges[i].first << " " << subgraphEdges[i].second << " " << subgraphWeights[i] << std::endl;
//       }
    }
    
    else if(std::string(argv[i]) == "-m2"){
      
      std::cout << "building graph..." << std::endl;
      std::vector<int> parents;
      std::vector<std::vector<std::pair<int,int>>> adjList = build_adjList(n, edgesWeights.first, edgesWeights.second);
      
      std::cout << "calculating steiner-tree..." << std::endl;
      for(auto j = 0lu, end=primes.size(); j<end; j++){
	
	if (j%500==0){
	  if (j+500-1 > primes.size()){
	    std::cout << "processing terminals number: " << j << " to " << primes.size() << std::endl;
	  }
	  else{
	    std::cout << "processing terminals number: " << j << " to " << j + (500-1) << std::endl;
	  }
	}
	
	bimap Unvisited = build_unvisited(n, currentPrime, subgraphEdges);
	
	std::pair<std::vector<int>,std::vector<int>> shortestDists = m2Algorithm(n, Unvisited, adjList, currentPrime);
	dists = shortestDists.first;
	parents = shortestDists.second;
	
	std::vector<int> primeDists = get_PrimeDists(dists, primes);
	std::pair<int,int> minPrimeIdxDist = get_Min(primeDists,primes);
	
// 	     std::cout << " ------- " << std::endl;
// 	      for (int i = 0; i<dists.size(); i++){
// 		//std::cout << i << " " << subgraphEdges[i].first << " " << subgraphEdges[i].second << std::endl;
// 		std::cout << i << " " << dists[i] << " " << parents[i] << std::endl;
// 	      }	
	
	
	int minPrimeIdx = minPrimeIdxDist.first;
	int minPrime = primes[minPrimeIdx];
	objVal += minPrimeIdxDist.second;
	
	int currentWeight = std::numeric_limits<int>::max();
	int currentNode = minPrime;
	int pred = parents[currentNode];
	
	//std::cout << "minPrime: " << currentNode << " pred: " << pred << std::endl;
	while(currentWeight != 0){
	  //std::cout << "current nodes: " << currentNode << " pred: " << pred << " currentweight: " << currentWeight << std::endl;
	  Edge edge = std::make_pair(pred,currentNode);
	  pred = parents[currentNode];
	  int predIdx;
	  int currentNodeIdx;
	  
	  //std::cout << "currentNode: " << currentNode << " pred: " << pred << std::endl;
	  
	  predIdx = get_NeighbourIndex(currentNode, pred, adjList);
	  currentWeight = adjList[currentNode][predIdx].second;
	  
	  if (currentWeight == 0){
	    break;
	  }
	  //std::cout << "current weight: " << currentWeight << std::endl;
	  
	  subgraphEdges.push_back(edge);
	  subgraphWeights.push_back(currentWeight);
	  
	  // change edge weight on both directions (undirected graph)
	  adjList[currentNode][predIdx].second = 0;
	  currentNodeIdx = get_NeighbourIndex(pred, currentNode,adjList);
	  adjList[pred][currentNodeIdx].second = 0;
	  
	  //std::cout << "current pred: " << pred << " with edge weight: " << currentWeight << std::endl;
	  currentNode = pred;
	  pred = parents[currentNode];
	  if (pred == currentNode){
	    break;
	  }
	}
	
	currentPrime = minPrime;
      }
      std::cout << "objVal = " << objVal << " edge count = " << subgraphEdges.size() << std::endl;
      std::cout << std::endl;
      
      std::cout << "checking steiner tree..." << std::endl;
      isConnected(subgraphEdges, startTerminal);
      isFeasible(subgraphEdges,primes);
    }
      //std::cout << "wip." << std::endl;
    }
//   for (int i=0; i<subgraphEdges.size(); i++){
//     std::cout << subgraphEdges[i].first << " " << subgraphEdges[i].second << std::endl;
//   }
  boost::timer::cpu_times times = timer.elapsed();
  /* end dijkstra algorithm according to flag */
  
  /// print CPU- and Wall-Time. 
  std::cout << std::endl;
  std::cout << "WALL-CLOCK " << times.wall / 1e9 << "s" << std::endl;
  std::cout << "USER TIME " << times.user / 1e9 << "s" << std::endl;
  std::cout << std::endl << std::endl;
  
  file.close();  
  return 0;
}
