#include "utils.h"
#include <map>

#ifndef MYHEAP_H
#define MYHEAP_H

class myHeap {
private:
  std::vector<vertex> tree;
  std::map<int,int> positions;
  
  void checkDown(int idx);
  
  void checkUp(int idx);
  
  void mkHeap();
  
public:
  myHeap(int& n, int& startTerminal, std::vector<Edge>& edges);
  
  void pop_top();
  
  vertex get_min();
  
  void insert(vertex& newVal);
  
  void update_weight(int& neighbourIdx, int& newWeight);

  int get_neighbourPosition(int& neighbour);
  
  int size();
  
  void print();
};

#endif