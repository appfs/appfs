#include "utils.h"
#include "heap.h"

#ifndef DIJKSTRA_ALG_H
#define DIJKSTRA_ALG_H

namespace dijkstra_alg {
  std::pair<std::vector<int>,std::vector<int>> alg (int& n, myHeap& Unvisited, std::vector<std::vector<Vertex>>& adjList, const int& startTerminal, std::vector<bool>& inSubgraph);
  
  int get_NeighbourIndex(int& currentNode, int& neighbour, std::vector<std::vector<Vertex>>& adjList);
}

#endif
