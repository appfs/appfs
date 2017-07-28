/** 
 *  @file    heap.cxx
 *  @author  Ray Chew
 *  @date    20 July 2017
 *  
 *  @brief Binary Heap structure.
 *
 */
// An attempt at a binary heap structure. Stores <value,index> pair of nodes which represents index on graph and distance from a certain node in the graph.
// Comes with `checkUp` and `checkDown` methods, which bubbles up and down the heap to sort the heap structure.
// `update_weight` allows for the changing of weights based on a index on the heap.
// The index on the heap can be obtained by the method `get_neighbourPosition`.
// Also returns the minimum (the top-most node) of the heap with `get_min` and removes this minimum with `pop_top`.

#include "heap.h"

using namespace std;

/** 
 *   @brief  Bubble down method of the myHeap class. Sorta heap at a given index downwards.
 *  
 *   @param  idx is an int that is the index of the node that should be sorted downwards.
 *   @return void
 */  
void myHeap::checkDown(int idx) {
  int length = tree.size();
  // get index of left child of the current index.
  int lChildIdx = 2*idx;
  
  // if the left child is outside of the tree, return since current-index node a leaf node.
  if (lChildIdx >= length) { 
    return;
  }
  
  // get index of right child of the current index.
  int rChildIdx = 2*idx+1;
  // get the value of the left child.
  int lChildWeight = tree[lChildIdx].first; 
  
  pair<int,int> *treeIdx = &tree[idx];
  int currentIdxWeight = treeIdx->first;
  int minIdx = idx;
  
  // initialise childWeight and childIdx to store lower child value of the left and right children.
  int childWeight = lChildWeight;
  int childIdx = lChildIdx;
  
  // if there is a right-child of current index node, then compare its value to the left child.
  if (rChildIdx < length) {
    int rChildWeight = tree[rChildIdx].first;
    
    // deterministical always choose to switch with the left child their values are the same.
    if (lChildWeight > rChildWeight) {
      childWeight = rChildWeight;
      childIdx = rChildIdx;
    }
  }
  
  // check if there's a need to switch by comparing value at the current index to the value at the chosen child.
  if (currentIdxWeight > childWeight) {
    minIdx = childIdx;
  }
  
  // if there is a need to switch nodes... switch the weights and update the positions array.
  if (minIdx != idx) {
    iter_swap(tree.begin() + idx, tree.begin() + minIdx);
    iter_swap(positions.begin() + treeIdx->second, positions.begin() + tree[minIdx].second);
    
    checkDown(minIdx); // check for the next (upper) levels recursively.
  }
}

/** 
 *   @brief  Bubble up method of the myHeap class. Sorta heap at a given index upwards.
 *  
 *   @param  idx is an int that is the index of the node that should be sorted upwards.
 *   @return void
 */  
void myHeap::checkUp(int idx) {
  // if current index is the root node, return since there's no bubbling up left to do.
  if (idx == 1) { 
    return;
  }
  
  int parentIdx = (idx)/2;
  
  int parentWeight = tree[parentIdx].first;
  int currentIdxWeight = tree[idx].first;
  
  // otherwise, check if the weight at parent node is larger than the weight at current node...
  if (parentWeight > currentIdxWeight) {
    
    iter_swap(tree.begin() + idx, tree.begin() + parentIdx);
    iter_swap(positions.begin() + tree[idx].second, positions.begin() + tree[parentIdx].second);
    
    checkUp(parentIdx);
  }
}

/** 
 *   @brief  Make the heap by bubbling down for each node on the heap.
 *
 *   @return void
 */  
void myHeap::mkHeap() {
  int length = tree.size();
  
  for(int i=length-1; i>0; --i) {
    checkDown(i);
  }
}

/** 
 *   @brief  myHeap constructor.
 * 
 *   @param  n as int is the number of nodes in the graph.
 *   @param  startTerminal as int is the index of the start terminal.
 *
 */  
myHeap::myHeap(int& n, const int& startTerminal) : tree(n) {
  positions.reserve(n); // initialise a positions array to "search" the binary heap by indices of the nodes.
  for (int i=0; i<n; i++) {
    tree[i] = make_pair(numeric_limits<int>::max(),i);
    positions[i] = i;
  }
  // the start node should have a weight of 0. "terminal" is used here for eventual steiner tree implementation.
  tree[startTerminal]=make_pair(0,startTerminal);
  
  mkHeap();
}

/** 
 *   @brief  Removes the top-most element on the heap, i.e. the node with the smallest value.
 *
 *   @return void
 */  
void myHeap::pop_top() {
  // if there is nothing left in tree, return.
  if (tree.size() == 1) { 
    return;
  }
  
  // not sure if it's better practice to use begin()/end() or to use the square brackets []...
  // update the positions array. Nodes that are no longer in the heap have positions of -inf.
  positions[(tree.end()-1)->second] = positions[(tree.begin()+1)->second]; 
  positions[(tree.begin()+1)->second] = numeric_limits<int>::min(); 
  
  // otherwise, switch last node with the root, bubble down and pop the last node, which is now the root.
  *(tree.begin()+1) = *(tree.end()-1);
  tree.pop_back();
  
  checkDown(1);
}

/** 
 *   @brief  Returns the top-most element on the heap, i.e. the node with the smallest value.
 *
 *   @return Vertex
 */  
Vertex myHeap::get_min() { // get the minimum (root node) of the binary heap.
  return tree[1];
}

/** 
 *   @brief  Inserts a new node into the heap and sort according to value.
 *
 *   @param  newNode as a Vertex with (weight,index).
 *   @return void
 */  
void myHeap::insert(Vertex newNode) {
  int length = tree.size();
  
  tree.push_back(newNode);
  positions[newNode.second] = length;
  checkUp(length);
}

/** 
 *   @brief  Updates the value of a given node.
 * 
 *   @param  neighbour is an int that is the index of the node on graph for which its index on the heap should be found.
 *   @param  newDist is an int that replaces the old value of a node.
 *   @return void
 */  
void myHeap::update_weight(int& neighbour, int& newDist) {
  int neighbourIdx = positions[neighbour];
  
  if (neighbourIdx != numeric_limits<int>::min()) { // if the neighbour is still in the heap, update.
    int oldWeight = tree[neighbourIdx].first;
    tree[neighbourIdx].first = newDist;  
    
    if (oldWeight <= newDist){
      checkDown(neighbourIdx);
    }
    else {
      checkUp(neighbourIdx);
    }
  }
  else { // if neighbour is no longer in heap, insert it back.
    insert(make_pair(newDist,neighbour));
  }
}

/** 
 *   @brief  Returns the index on the heap of a given index on a graph, i.e. to find where a certain node on the graph is on the heap.
 * 
 *   @param  neighbour is an int that is the index of the node on graph for which its index on the heap should be returned.
 *   @return void
 */  
int* myHeap::get_neighbourPosition(int& neighbour) {
  return &positions[neighbour];
}

/** 
 *   @brief  Returns the size of the heap.
 * 
 *   @return int
 */  
int myHeap::size() {
  return tree.size();
}

/** 
 *   @brief  Prints the tree and positions array of the heap. For debugging purposes. Left here for future reference.
 * 
 *   @return void
 */  
void myHeap::print() {
  for(int i=0, end=tree.size(); i<end; ++i)
    cout << "(idx" << i << ": " << tree[i].first << "," << tree[i].second << ")" << ' ';
  cout << endl;
  for(int i=0, end=positions.size(); i<end; ++i)
    cout << "(" << i << " : " << positions[i] << ")" << ' ';
  cout << endl;  
}