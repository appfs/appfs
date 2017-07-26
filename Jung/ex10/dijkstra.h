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

//typedegs for edges
typedef std::pair<int, int> Edge_wW;


void bubbleDown(int* heap, int* whereInHeap, int* whichNode,int nodePos, int heapLength);
void bubbleUp(int* heap, int* whereInHeap, int* whichNode,int nodePos);
int dijkstra(std::vector<Edge_wW>* edges, bool* steinTree, bool* used, int* pre, int vertNumb, 
			int restNodes, std::vector<int> &steinerNodes, int heapLength);

#endif