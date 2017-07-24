#include "utils.h"

#ifndef MYHEAP_H
#define MYHEAP_H

class myHeap {
private:
  std::vector<Vertex> tree;
  std::vector<int> positions;
  
  void checkDown(int idx);
  
  void checkUp(int idx);
  
  void mkHeap();
  
public:
  myHeap(int& n, const int& startTerminal, std::vector<bool>& inSubgraph);
  
  void pop_top();
  
  Vertex get_min();
  
  void insert(Vertex& newVal);
  
  void update_weight(int* neighbourIdx, int& newWeight);

  int* get_neighbourPosition(int& neighbour);
  
  int size();
  
  void print();
};

#endif