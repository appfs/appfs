/** 
 *  @file    steiner.h
 *  @author  Ray Chew
 *  @date    20 July 2017
 *  
 *  @brief steiner namespace header containing function that runs the main Dijkstra algorithm and Steiner heuristics.
 *
 */

#include "utils.h"
#include "heap.h"

#ifndef STEINER_H
#define STEINER_H

/**
 * @brief steiner namespace: contains function alg(). Runs the main Dijkstra algorithm and Steiner heuristics.
 */
namespace steiner {
  std::pair<std::vector<Edge>,std::vector<int>> alg (int& n, myHeap& Unvisited, std::vector<std::vector<Vertex>>& adjList, const int& startTerminal, std::vector<bool>& isPrimes);
  
}

#endif
