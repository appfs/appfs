#include "dijkstra_alg.h"

vertex dijkstra_alg::alg (int& n, myHeap& Unvisited, std::vector<std::vector<vertex>>& adjList, int& startTerminal, std::vector<Edge>& edges) {
  std::vector<vertex> finalWeights(n); // initialise lists to store the weights and parents of current dijkstra iteration.
  std::vector<int> finalParents(n);
  
  for(int j=0, end=n; j<end; j++) { // all weights except that of the start node should have a weight of infinity.
    finalWeights[j] = std::make_pair(j, std::numeric_limits<int>::max());
  }
  
  finalWeights[startTerminal].second = 0;
  finalParents[startTerminal] = startTerminal;
  
  /* start calculating and updating distances */
  while(Unvisited.size()>0) { // main dijkstra algorithm. ref: https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm#Pseudocode
    vertex minPair = Unvisited.get_min();
    int minIdx = minPair.second;
    int minDist = minPair.first;
    signed int adjListSize = adjList[minIdx].size();
    
    for(int j=0; j<adjListSize; j++) {
      int neighbour = adjList[minIdx][j].first;
      int dist = adjList[minIdx][j].second;
      
      int newDist = minDist + dist;
      int nPWeight = finalWeights[neighbour].second;
      
      if (newDist < nPWeight) {
	int toBeReplaced = Unvisited.get_neighbourPosition(neighbour);
	if (toBeReplaced != std::numeric_limits<int>::min()){
	  Unvisited.update_weight(toBeReplaced,newDist);
	}
	finalWeights[neighbour].second = newDist;
	finalParents[neighbour] = minIdx;
      }
    }
    Unvisited.pop_top();
  }
  
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
