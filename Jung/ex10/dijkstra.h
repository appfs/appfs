#ifndef DIJKSTRA
#define DIJKSTRA


#include <iostream>
#include <fstream>
#include <string>
#include <exception>
#include <vector>
#include <time.h>
#include <utility>
#include <limits>
#include <tuple>
#include <list>
#include <climits>
#include <cmath>
#include "utils.h"


void dijkstra(Edg &edges, bool* steinTree, int* pre, int &vertNumb,
		int start, std::vector<std::pair<int,int>> &steinEdges, int &heapLength, bool* isPrime,int primeNumb, long long &weightST);

#endif
