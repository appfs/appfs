#include "utils.h"
#include "heap.h"

#ifndef DIJKSTRA_ALG_H
#define DIJKSTRA_ALG_H

namespace dijkstra_alg {

  vertex alg (int& n, std::vector<std::vector<vertex>>& adjList, int& startTerminal, std::vector<Edge> subgraphEdges);

  int get_NeighbourIndex(int& currentNode, int& neighbour, std::vector<std::vector<vertex>>& adjList);
};

#endif
