#include "heap.h"

using namespace std;

void myHeap::checkDown(int idx) { // the bubble down method.
  int length = tree.size();
  int lChildIdx = 2*idx; // get index of left and right child of the current index.

  if (lChildIdx >= length) { // if the left child is outside of the tree, return since current-index node a leaf node.
    return;
  }
  
  int rChildIdx = 2*idx+1;
  int lChildWeight = tree[lChildIdx].first; // otherwise, get the indices.

  int currentIdxWeight = tree[idx].first;
  int minIdx = idx;
  
  int childWeight = lChildWeight;
  int childIdx = lChildIdx;
  
  if (rChildIdx < length) { // if there is a right-child of current index node, then compare its weight to the left child.
    int rChildWeight = tree[rChildIdx].first;
    
    if (lChildWeight > rChildWeight) { // deterministical always choose to switch with the left child if values are the same.
      childWeight = rChildWeight;
      childIdx = rChildIdx;
    }
  }
//   else { // otherwise, the only child weight is that of the left child.
//     childWeight = lChildWeight;
//     childIdx = lChildIdx;
//   }
  
  if (currentIdxWeight > childWeight) { // check if there's a need to switch by comparing weight at the current index to the weight at the chosen child.
    minIdx = childIdx;
  }  
//   if (lChildIdx >= length) { // if the left child is outside of the tree, return since current-index node a leaf node.
//     return;
//   }
//   
//   int lChildWeight = tree[lChildIdx].first; // otherwise, get the indices.
//   int rChildWeight = tree[rChildIdx].first;
//     
//   int currentIdxWeight = tree[idx].first;
//   int minIdx = idx;
//   int childWeight = lChildWeight;
//   int childIdx = rChildWeight;
//   
//   //if (rChildIdx < length) { // if there is a right-child of current index node, then compare its weight to the left child.
//   //int rChildWeight = tree[rChildIdx].first;
//   
//   if (lChildWeight > rChildWeight) { // deterministical always choose to switch with the left child if values are the same.
//     childWeight = rChildWeight;
//     childIdx = rChildIdx;
//   }
// //   else if (lChildWeight <= rChildWeight) {
// //     childWeight = lChildWeight;
// //     childIdx = lChildIdx;
// //   }
//   //}
// //   else { // otherwise, the only child weight is that of the left child.
// //     childWeight = lChildWeight;
// //     childIdx = lChildIdx;
// //   }
//   
//   if (currentIdxWeight > childWeight) { // check if there's a need to switch by comparing weight at the current index to the weight at the chosen child.
//     minIdx = childIdx;
//   }
  
  if (minIdx != idx) { // if there is a need to switch nodes... switch the weights and update the positions array.
    
//     pair<int,int> tmp = tree[idx];
//     pair<int,int> *treeIdx = &tree[idx];
//     pair<int,int> *treeMinIdx = tree[minIdx];
//     *treeIdx = *treeMinIdx;
//     *treeMinIdx = tmp;
    
    iter_swap(tree.begin() + idx, tree.begin() + minIdx);
    iter_swap(positions.begin() + tree[idx].second, positions.begin() + tree[minIdx].second);
//     auto tmp1 = positions[treeIdx->second];
//     positions[treeIdx->second] = positions[treeMinIdx->second];
//     positions[treeMinIdx->second] = tmp1;
    checkDown(minIdx);
  }
}

void myHeap::checkUp(int idx) { // the bubbleUp method.
  if (idx == 1) { // if current index is the root node, return since there's no bubbling up left to do.
    return;
  }
  
  int parentIdx = (idx)/2;
  
  int parentWeight = tree[parentIdx].first;
  int currentIdxWeight = tree[idx].first;
  
  if (parentWeight > currentIdxWeight) { // otherwise, check if the weight at parent node is larger than the weight at current node...
    
//     pair<int,int> tmp = tree[parentIdx]; // if yes, switch them, and update the positions array.
//     pair<int,int> *treeIdx = &tree[idx];
//     pair<int,int> *treeParentIdx = &tree[parentIdx];
//     *treeParentIdx = *treeIdx;
//     *treeIdx = tmp;
//     
//     int tmpPos = positions[treeParentIdx->second];
//     positions[treeParentIdx->second] = positions[treeIdx->second];
//     positions[treeIdx->second] = tmpPos;
    
    iter_swap(tree.begin() + idx, tree.begin() + parentIdx);
    iter_swap(positions.begin() + tree[idx].second, positions.begin() + tree[parentIdx].second);
    
    checkUp(parentIdx);
  }
}

void myHeap::mkHeap() { // make the heap by bubbling downwards for each node in binary heap after it is added.
  int length = tree.size();
  
  for(int i=length-1; i>0;--i) {
    checkDown(i);
  }
}

myHeap::myHeap(int& n, int& startTerminal, std::vector<Edge>& edges) : tree(n) {
  positions.reserve(n);
  for (int i=1;i<n;i++) {
    tree[i] = make_pair(numeric_limits<int>::max(),i); // initialise binary heap as infinity weight for all nodes except the start node of the graph.
    positions[i] = i; // initialise a positions array to "search" the binary heap by indices of the nodes.
  }
  tree[startTerminal]=make_pair(0,startTerminal); // the start node should have a weight of 0. "terminal" is used here for eventual steiner tree implementation.
  
  mkHeap();
}

void myHeap::pop_top() { // remove the minimum (root node) of the binary heap.
  int length = tree.size();
  
  if(length == 0) // if there is nothing left in tree, return.
  {
    return;
  }
  pair<int,int> *treeFirst = &tree[1];
  pair<int,int> *treeLast = &tree[length-1];
  
  positions[treeLast->second] = positions[treeFirst->second]; // update the positions array.
  positions[treeFirst->second] = numeric_limits<int>::min(); // nodes that are no longer in the heap have positions of -inf.
  
  *treeFirst = *treeLast; // otherwise, switch last node with the root and bubble down.
  tree.pop_back(); // and pop the last node, which is now the root.
  
  checkDown(1);
}

pair<int,int> myHeap::get_min() { // get the minimum (root node) of the binary heap.
  return tree[1];
}

void myHeap::insert(pair<int,int>& newVal) { // unnecessary method to insert new elements into the heap, and keep sorted structure.
  int length = tree.size();
  tree.push_back(newVal);
  checkUp(length);
}

void myHeap::update_weight(int& neighbour, int& newWeight) {// given a new weight and the position of the neighbouring node on the binary heap, update the weight of this neighbouring node.
  //int *treeNeighbourWeightAddr = &tree[*neighbourIdx].first;
  int neighbourIdx = positions[neighbour];
  if (neighbourIdx != numeric_limits<int>::min()) {
  int oldWeight = tree[neighbourIdx].first;
  tree[neighbourIdx].first = newWeight;  
  if (oldWeight<=newWeight){
    checkDown(neighbourIdx);
  }
  else {
    checkUp(neighbourIdx);
  }
  }
  //if (*treeNeighbourWeightAddr<=newWeight){
//   if (tree[*neighbourIdx].first<=newWeight){
//     checkDown(*neighbourIdx);
//   }
//   else {
//     checkUp(*neighbourIdx);
//   }
//   //*treeNeighbourWeightAddr = newWeight;
//   tree[*neighbourIdx].first = newWeight;
}

int* myHeap::get_neighbourPosition(int& neighbour) { // given the index of the neighbouring node on the graph, return the position/index of the neighbouring node on the binary heap.
  return &positions[neighbour];
}

int myHeap::size() { // get the size of the binary heap.
  return tree.size();
}

void myHeap::print() { // print the binary heap and the positions array. For debugging purposes.
  for(int i=0, end=tree.size(); i<end; ++i)
    cout << "(idx" << i << ": " << tree[i].first << "," << tree[i].second << ")" << ' ';
  cout << endl;
  for(int i=0, end=positions.size(); i<end; ++i)
    cout << "(" << i << " : " << positions[i] << ")" << ' ';
  cout << endl;  
}