/** 
 *  @file    checker.h
 *  @author  Ray Chew
 *  @date    20 July 2017
 *  
 *  @brief checker namespace header containing the functions to check solutions of steiner subgraph.
 *
 */

#include <queue> /* for std::queue */
#include "utils.h"

#ifndef CHECKER_H
#define CHECKER_H

/**
 * @brief checker namespace: contains functions isTree(), isFeasible(). Checks if solutions are valid.
 */
namespace checker {
bool isTree(std::vector<Edge>& edges, const int& startTerminal);

bool isFeasible(std::vector<Edge>& edges, std::vector<int>& primes);
}

#endif