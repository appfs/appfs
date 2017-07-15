#include "heap.h"

using namespace std;

void myHeap::checkDown(int idx) { // reference: https://www.codeproject.com/Tips/816934/Min-Binary-Heap-Implementation-in-Cplusplus
  int length = tree.size();
  int lChildIdx = 2*idx+1;
  int rChildIdx = 2*idx+2;
  
  if (lChildIdx >= length) {
    return;
  }
  int lChildWeight = tree[lChildIdx].first;
  int rChildWeight = tree[rChildIdx].first;
  int currentIdxWeight = tree[idx].first;
  
  int minIdx = idx;
  int childWeight = numeric_limits<int>::max();
  int childIdx = numeric_limits<int>::max();
  
  if ((lChildWeight > rChildWeight) && (rChildIdx < length)) { // always choose left child if values are the same.
    childWeight = rChildWeight;
    childIdx = rChildIdx;
  }
  else if (lChildWeight <= rChildWeight) {
    childWeight = lChildWeight;
    childIdx = lChildIdx;
  }
  
  if (currentIdxWeight > childWeight) {
    minIdx = childIdx;
  }
  
  if (minIdx != idx) {
    pair<int,int> tmp = tree[idx];
    tree[idx] = tree[minIdx];
    tree[minIdx] = tmp;
    
    int tmp1 = positions[tree[idx].second];
    positions[tree[idx].second] = positions[tree[minIdx].second];
    positions[tree[minIdx].second] = tmp1;
    checkDown(minIdx);
  }
}

void myHeap::checkUp(int idx) {
  if (idx == 0) {
    return;
  }
  
  int parentIdx = (idx-1)/2;
  
  int parentWeight = tree[parentIdx].first;
  int currentIdxWeight = tree[idx].first;
  if (parentWeight > currentIdxWeight) {
    pair<int,int> tmp = tree[parentIdx];
    tree[parentIdx] = tree[idx];
    tree[idx] = tmp;
    
    int tmpPos = positions[tree[parentIdx].second];
    positions[tree[parentIdx].second] = positions[tree[idx].second];
    positions[tree[idx].second] = tmpPos;
    
    checkUp(parentIdx);
  }
}

void myHeap::mkHeap() {
  int length = tree.size();
  
  for(int i=length-1; i>=0;--i) {
    checkDown(i);
  }
}

myHeap::myHeap(int& n, int& startTerminal, std::vector<Edge>& edges) : tree(n) {
  for (int i=1;i<n;i++) {
    tree[i] = make_pair(numeric_limits<int>::max(),i);
    positions[i] = i;
  }
  tree[startTerminal]=make_pair(0,startTerminal);
  
  std::vector<int> nodes = utils::get_Nodes(edges);
  if (!nodes.empty()) {
    for(int j=0, end=nodes.size(); j<end; j++) {
      tree[nodes[j]] = make_pair(0,nodes[j]);
    }
  }
  mkHeap();
}

void myHeap::pop_top() {
  int length = tree.size();
  
  if(length == 0)
  {
    return;
  }
  tree[0] = tree[length-1];
  tree.pop_back();
  
  positions[tree[length-1].second] = positions[tree[0].second];
  positions[tree[0].second] = numeric_limits<int>::min();
  
  checkDown(0);
}

pair<int,int> myHeap::get_min() {
  return tree[0];
}

void myHeap::insert(pair<int,int>& newVal) {
  int length = tree.size();
  tree.push_back(newVal);
  checkUp(length);
}

void myHeap::update_weight(int& neighbourIdx, int& newWeight) {
  int oldWeight = tree[neighbourIdx].first;
  tree[neighbourIdx].first = newWeight;
  if (oldWeight<=newWeight){
    checkDown(neighbourIdx);
  }
  if (newWeight<oldWeight){
    checkUp(neighbourIdx);
  }
}

int myHeap::get_neighbourPosition(int& neighbour) {
  return positions[neighbour];
}

int myHeap::size() {
  return tree.size();
}

void myHeap::print() {
  for(int i=0, end=tree.size(); i<end; ++i)
    cout << "(idx" << i << ": " << tree[i].first << "," << tree[i].second << ")" << ' ';
  cout << endl;
//   for(int i=0, end=positions.size(); i<end; ++i)
//     cout << "(" << i << " : " << positions[i] << ")" << ' ';
//   cout << endl;  
}