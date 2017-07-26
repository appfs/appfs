#include <queue> /* for std::queue */
#include "utils.h"

#ifndef CHECKER_H
#define CHECKER_H

namespace checker {
bool isTree(std::vector<Edge>& edges, const int& startTerminal);

bool isFeasible(std::vector<Edge>& edges, std::vector<int>& primes);
}

#endif