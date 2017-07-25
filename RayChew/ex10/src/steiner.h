#include "utils.h"
#include "heap.h"

#ifndef STEINER_H
#define STEINER_H

namespace steiner {
  std::pair<std::vector<Edge>,std::vector<int>> alg (int& n, myHeap& Unvisited, std::vector<std::vector<Vertex>>& adjList, const int& startTerminal, std::vector<bool>& isPrime);
  
}

#endif
