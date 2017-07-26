#include "dijkstra_alg.h"

vertex dijkstra_alg::alg (int& n, std::vector<std::vector<vertex>>& adjList, int& startTerminal, std::vector<Edge> subgraphEdges) {
  boost::timer::cpu_timer timer;
  std::vector<int> finalWeights(n,std::numeric_limits<int>::max()); // initialise lists to store the weights and parents of current dijkstra iteration.
  std::vector<int> finalParents(n);
  
  myHeap Unvisited(n, startTerminal, subgraphEdges); // build prioritiy queue using heap structure.
  finalWeights[startTerminal] = 0;
  finalParents[startTerminal] = startTerminal;
  
  boost::timer::cpu_times times0 = timer.elapsed();
  /* start calculating and updating distances */
  while(Unvisited.size()>0) { // main dijkstra algorithm. ref: https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm#Pseudocode
    vertex minPair = Unvisited.get_min(); // get the node with the minimum weight on the queue
    int minIdx = minPair.second; // get its index,
    
    std::vector<std::pair<int,int>> *neighboursOfMinIdx = &adjList[minIdx]; // then get the neighbours of this "minimum" node from the adjacency list.
    
    for(auto j = neighboursOfMinIdx->begin(), end = neighboursOfMinIdx->end(); j!=end; j++) { // for each neighbour,
      int neighbour = j->first; // get the node-index of the neighbour on the graph
      int dist = j->second; // and its distance to the "minimum" node.
      
      int newDist = minPair.first + dist;
      if (newDist < finalWeights[neighbour]) {
	  Unvisited.update_weight(neighbour,newDist); // update its weight (distance to the start terminal) to be the newly calculated distance.

	finalWeights[neighbour] = newDist; // then for this neighbouring node, update the final distance,
	finalParents[neighbour] = minIdx; // and note that its parent is the current "minimum" node.
      }
    }
    Unvisited.pop_top(); // remove the minimum value (at the top) of the priority queue.
  }
  boost::timer::cpu_times times1 = timer.elapsed();
  std::cout << "WALL-CLOCK " << times0.wall / 1e9 << "s" << std::endl;
  std::cout << "USER TIME " << times0.user / 1e9 << "s" << std::endl;
  std::cout << "WALL-CLOCK " << times1.wall / 1e9 << "s" << std::endl;
  std::cout << "USER TIME " << times1.user / 1e9 << "s" << std::endl;
  int maxDistance = 0;
  int maxVertex = 0;
  

  for(int i=0, end=finalWeights.size(); i<end; i++) {
    /// replace maxDistance if a greater distance is found, and maxDistance must be less than "infinity" (of 32-bit signed integer).
    if ((finalWeights[i] > maxDistance) && (finalWeights[i] < std::numeric_limits<int>::max())) {
      maxDistance = finalWeights[i];
      maxVertex = i;
    }
    /// if distance == maxDistance, check if vertex index is smaller.
    else if ((finalWeights[i] == maxDistance) && (i < maxVertex)) {
      maxVertex = i;
    }
  }
  /* end find node of the longest-shortest path */
  
  vertex Final = std::make_pair(maxVertex,maxDistance);
  return Final;
  /* end calculating and updating distances */
}

int dijkstra_alg::get_NeighbourIndex(int& currentNode, int& neighbour, std::vector<std::vector<vertex>>& adjList) { // method that finds the index of the neighbouring node on the adjacency list, given the index of the current node and the neighbour node on the graph.
  int neighbourIdx = 0;
  for (auto i=adjList[currentNode].begin(), end=adjList[currentNode].end(); i!=end; i++) {
    if (i->first==neighbour) {
      break;
    }
    neighbourIdx++;
  }
  return neighbourIdx;
}
