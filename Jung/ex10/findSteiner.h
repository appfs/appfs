#ifndef FINDSTEINER
#define FINDSTEINER



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
#include <omp.h>
#include "dijkstra.h"

// first the edge pointing to second the weight
typedef std::pair<int,int> Edge_wW;

void copyArray(bool* copyTo, bool* copyFrom, int arrSize);
void updateSteiner(std::vector<Edge_wW>* nodes, bool* steinTree, std::vector<int> &steinerNodes, int* pre, int* dijPre, int vertNumb, std::vector<Edge_wW> &steinEdges);
long long findSteiner(std::vector<Edge_wW> nodes[],int vertNumb, int primeNumb, std::vector<Edge_wW> &steinEdges, bool* steinTree, int start);




#endif