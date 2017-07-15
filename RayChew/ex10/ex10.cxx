/**
 * \file ex10.cxx
 * \author Ray Chew
 * \date 15 July 2017
 * \brief Steiner-Tree builder for undirected .gph graphs
 */

/**
 * \mainpage Ex10
 * 
 * \section Description
 * 
 * * compile: `g++ -std=c++14 utils.cxx checker.cxx boost_dijkstra.cxx m2alg.cxx ex8.cxx -o ex8 -lboost_timer -Wall`<br>
 * * OR: just use make.
 * * run: `./ex10 filepath/graph.gph [source node]`<br>
 */

#include "utils.h"
#include "checker.h"
#include "heap.h"
#include "dijkstra_alg.h"
#include <stdlib.h> 

int main(int argc, char* argv[]) {
  std::ifstream file(argv[1]);  // read graph file.
  std::string str; /// read graph file line by line.
  int startTerminal = std::atoi(argv[2]);
  
  
  /* start get number of edges */
  getline(file, str);
  int n; /// store n as int for number of edges.
  
  auto it = str.begin();
  parse(it, str.end(), int_[([&n](int i){n = i;})] >> int_);
  n++;
  /* end get number of edges */
  
  std::vector<int> primes = utils::gen_Primes(n);
  std::pair<std::vector<Edge>,std::vector<int>> edgesWeights = utils::get_EdgesWeights(n, file);
  
  std::vector<Edge> subgraphEdges;
  std::vector<int> subgraphWeights;
  std::vector<int> dists;
  
  int objVal = 0;
  int currentPrime = startTerminal;
  std::vector<int> visitedPrimes;
  
  std::cout << "building graph..." << std::endl;
  std::vector<int> parents;
  std::vector<std::vector<std::pair<int,int>>> adjList = utils::build_adjList(n, edgesWeights.first, edgesWeights.second);
  
  std::cout << "calculating steiner-tree..." << std::endl;
  boost::timer::cpu_timer timer;
  for(auto j = 0lu, end=primes.size(); j<end; j++) {
   // std::cout << j+1 << " of " << primes.size() << " currentPrime: " << currentPrime << std::endl;
    if (j%500==0){
      if (j+500-1 > primes.size()){
	std::cout << "processing terminals number: " << j << " to " << primes.size() << std::endl;
      }
      else{
	std::cout << "processing terminals number: " << j << " to " << j + (500-1) << std::endl;
      }
    }
    
    myHeap Unvisited(n, currentPrime, subgraphEdges);
    
    std::pair<std::vector<int>,std::vector<int>> shortestDists = dijkstra_alg::alg(n, Unvisited, adjList, currentPrime, subgraphEdges);
    dists = shortestDists.first;
    parents = shortestDists.second;
    
    std::vector<int> primeDists = utils::get_PrimeDists(dists, primes);
    std::pair<int,int> minPrimeIdxDist = utils::get_Min(primeDists,primes);
    
    int minPrimeIdx = minPrimeIdxDist.first;
    int minPrime = primes[minPrimeIdx];
    if (minPrime == 47){
      std::cout << "prime distances: ";
    for (int i=0,end=primeDists.size();i<end;i++){
      std::cout << " (" << primes[i] << " " << primeDists[i] << ") ";
    }
    std::cout << std::endl;      
            std::cout << "subgraphEdges: ";
    for (int i=0,end=subgraphEdges.size();i<end;i++){
      std::cout << " (" << subgraphEdges[i].first << "," << subgraphEdges[i].second << "), ";
    }
   std::cout << std::endl;
    }
    objVal += minPrimeIdxDist.second;
    //std::cout<<objVal<<" "<<minPrimeIdxDist.first<<" "<<minPrimeIdxDist.second<<std::endl;
    int currentWeight = std::numeric_limits<int>::max();
    int currentNode = minPrime;
    int pred = parents[currentNode];
    
//     std::cout << "dists & parents: ";
//     for (int i=0,end=parents.size();i<end;i++){
//       std::cout << " (" << i << " " << dists[i] << " " << parents[i] << ") ";
//     }
//     std::cout << std::endl;
    if (currentPrime == 67){
      std::cout << "prime distances: ";
    for (int i=0,end=primeDists.size();i<end;i++){
      std::cout << " (" << primes[i] << " " << primeDists[i] << ") ";
    }
    std::cout << std::endl;      
            std::cout << "subgraphEdges: ";
    for (int i=0,end=subgraphEdges.size();i<end;i++){
      std::cout << " (" << subgraphEdges[i].first << "," << subgraphEdges[i].second << "), ";
    }
    }
    while(currentWeight != 0) {
      Edge edge = std::make_pair(pred,currentNode);
      int predIdx;
      int currentNodeIdx;
      
      //std::cout << "currentNode: " << currentNode << " pred: " << pred << std::endl;
      predIdx = dijkstra_alg::get_NeighbourIndex(currentNode, pred, adjList);
      
      currentWeight = adjList[currentNode][predIdx].second;
      //std::cout << "pred: " << pred << " currentNode: " << currentNode << " currentWeight: " << currentWeight << std::endl;
      //std::cout << "currentWeight: " << currentWeight << std::endl;
      if (currentWeight == 0) {
	break;
      }
      subgraphEdges.push_back(edge);
      //std::cout << "pushed back!" << std::endl;
      subgraphWeights.push_back(currentWeight);
      
      // change edge weight on both directions (undirected graph)
      adjList[currentNode][predIdx].second = 0;
      currentNodeIdx = dijkstra_alg::get_NeighbourIndex(pred, currentNode,adjList);
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
//}
boost::timer::cpu_times times = timer.elapsed();

/// print CPU- and Wall-Time. 
std::cout << std::endl;
std::cout << "WALL-CLOCK " << times.wall / 1e9 << "s" << std::endl;
std::cout << "USER TIME " << times.user / 1e9 << "s" << std::endl;
std::cout << std::endl << std::endl;

file.close();  
return 0;
}
