/** 
 *  @file    heap.h
 *  @author  Ray Chew
 *  @date    20 July 2017
 *  
 *  @brief Header file for Binary Heap structure.
 *
 */

#include "utils.h"

#ifndef MYHEAP_H
#define MYHEAP_H

/**
 * @brief myHeap class: initialises an instance of a binary heap with methods: pop_top(), get_min(), insert(), update_weight(), size().
 */
class myHeap {
private:
  std::vector<Vertex> tree; ///< vector<Vertex> to store all nodes on the graph as std::pair of (weight,index).
  std::vector<int> positions; ///< vector<int> to store the position in the heap of a node on the graph.
  
  void checkDown(int idx);
  
  void checkUp(int idx);
  
  void mkHeap();
  
public:
  myHeap(int& n, const int& startTerminal);
  
  void pop_top();
  
  Vertex get_min();
  
  void insert(Vertex newVal);
  
  void update_weight(int& neighbour, int& newWeight);
  
  int* get_neighbourPosition(int& neighbour);
  
  int size();
  
  void print();
};

#endif