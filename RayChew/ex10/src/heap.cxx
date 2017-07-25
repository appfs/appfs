/** 
 *  @file    heap.cxx
 *  @author  Ray Chew
 *  @date    20 July 2017
 *  
 *  @brief Binary Heap structure
 *
 *  @section Description
 *  
 *  An attempt at a binary heap structure. Stores <value,index> pair of nodes which represents index on graph and distance from a certain node in the graph.
 *  * Comes with `checkUp` and `checkDown` methods, which bubbles up and down the heap to sort the heap structure.
 *  * `update_weight` allows for the changing of weights based on a index on the heap.
 *  * The index on the heap can be obtained by the method `get_neighbourPosition`.
 *  * Also returns the minimum (the top-most node) of the heap with `get_min` and removes this minimum with `pop_top`.
 *
 */

#include "heap.h"

using namespace std;

/** 
 *   @brief  Bubble down method of the myHeap class. Sorta heap at a given index downwards.
 *  
 *   @param  idx is an int that is the index of the node that should be sorted downwards.
 *   @return void
 */  
void myHeap::checkDown(int idx) { // the bubble down method.
  int length = tree.size();
  int lChildIdx = 2*idx; // get index of left and right child of the current index.
  
  if (lChildIdx >= length) { // if the left child is outside of the tree, return since current-index node a leaf node.
    return;
  }
  
  int rChildIdx = 2*idx+1;
  int lChildWeight = tree[lChildIdx].first; // otherwise, get the index of the left child.
  
  pair<int,int> *treeIdx = &tree[idx];
  int currentIdxWeight = treeIdx->first;
  int minIdx = idx;
  
  int childWeight = lChildWeight; // initialise childWeight and childIdx to store lower child value of the left and right children.
  int childIdx = lChildIdx;
  
  if (rChildIdx < length) { // if there is a right-child of current index node, then compare its value to the left child.
    int rChildWeight = tree[rChildIdx].first; // by first getting the index of the right child.
    
    if (lChildWeight > rChildWeight) { // deterministical always choose to switch with the left child their values are the same.
      childWeight = rChildWeight;
      childIdx = rChildIdx;
    }
  }
  
  if (currentIdxWeight > childWeight) { // check if there's a need to switch by comparing value at the current index to the value at the chosen child.
    minIdx = childIdx;
  }
  
  if (minIdx != idx) { // if there is a need to switch nodes... switch the weights and update the positions array.
    iter_swap(tree.begin() + idx, tree.begin() + minIdx);
    iter_swap(positions.begin() + treeIdx->second, positions.begin() + tree[minIdx].second);
    
    checkDown(minIdx); // check for the next (upper) levels recursively.
  }
}

/** 
 *   @brief  Bubble down method of the myHeap class. Sorta heap at a given index downwards.
 *  
 *   @param  idx is an int that is the index of the node that should be sorted downwards.
 *   @return void
 */  
void myHeap::checkUp(int idx) { // the bubbleUp method.
  if (idx == 1) { // if current index is the root node, return since there's no bubbling up left to do.
    return;
  }
  
  int parentIdx = (idx)/2;
  
  int parentWeight = tree[parentIdx].first;
  int currentIdxWeight = tree[idx].first;
  
  if (parentWeight > currentIdxWeight) { // otherwise, check if the weight at parent node is larger than the weight at current node...
    
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
void myHeap::mkHeap() { // make the heap by bubbling downwards for each node in binary heap after it is added.
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
 *   @param  inSubgraph is a vector<bool> where true corresponds to nodes in the steiner subgraph and false otherwise.
 *
 */  
myHeap::myHeap(int& n, const int& startTerminal) : tree(n) {
  positions.reserve(n);
  for (int i=0; i<n; i++) {
    tree[i] = make_pair(numeric_limits<int>::max(),i);
    positions[i] = i; // initialise a positions array to "search" the binary heap by indices of the nodes.
  }
  tree[startTerminal]=make_pair(0,startTerminal); // the start node should have a weight of 0. "terminal" is used here for eventual steiner tree implementation.
  
  mkHeap();
}

/** 
 *   @brief  Removes the top-most element on the heap, i.e. the node with the smallest value.
 *
 *   @return void
 */  
void myHeap::pop_top() { // remove the minimum (root node) of the binary heap.
  int length = tree.size();
  
  if(length == 0) // if there is nothing left in tree, return.
  {
    return;
  }
  
  positions[(tree.end()-1)->second] = positions[tree.begin()->second]; // update the positions array.
  positions[tree.begin()->second] = numeric_limits<int>::min(); // nodes that are no longer in the heap have positions of -inf.
  
  *tree.begin() = *(tree.end()-1); // otherwise, switch last node with the root and bubble down.
  tree.pop_back(); // and pop the last node, which is now the root.
  
  checkDown(0);
}

/** 
 *   @brief  Returns the top-most element on the heap, i.e. the node with the smallest value.
 *
 *   @return Vertex
 */  
Vertex myHeap::get_min() { // get the minimum (root node) of the binary heap.
  return tree[0];
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
 *   @param  neighbour is an int that is the index of the node on graph for which its index on the heap should be returned.
 *   @param  newDist is an int that replaces the old value of a node.
 *   @return void
 */  
void myHeap::update_weight(int& neighbour, int& newDist) { // given a new weight and the position of the neighbouring node on the binary heap, update the weight of this neighbouring node.
  int neighbourIdx = positions[neighbour];
  if (neighbourIdx != numeric_limits<int>::min()) {
    int oldWeight = tree[neighbourIdx].first;
    tree[neighbourIdx].first = newDist;  
    if (oldWeight <= newDist){
      checkDown(neighbourIdx);
    }
    else {
      checkUp(neighbourIdx);
    }
  }
//   else {
//     insert(make_pair(newDist,neighbour));
//   }
}

/** 
 *   @brief  Returns the index on the heap of a given index on a graph, i.e. to find where a certain node on the graph is on the heap.
 * 
 *   @param  neighbour is an int that is the index of the node on graph for which its index on the heap should be returned.
 *   @return void
 */  
int* myHeap::get_neighbourPosition(int& neighbour) { // given the index of the neighbouring node on the graph, return the position/index of the neighbouring node on the binary heap.
  return &positions[neighbour];
}

/** 
 *   @brief  Returns the size of the heap.
 * 
 *   @return int
 */  
int myHeap::size() { // get the size of the binary heap.
  return tree.size();
}

/** 
 *   @brief  Prints the tree and positions array of the heap. For debugging purposes. Left here for future reference.
 * 
 *   @return void
 */  
void myHeap::print() { // print the binary heap and the positions array. For debugging purposes.
  for(int i=0, end=tree.size(); i<end; ++i)
    cout << "(idx" << i << ": " << tree[i].first << "," << tree[i].second << ")" << ' ';
  cout << endl;
  for(int i=0, end=positions.size(); i<end; ++i)
    cout << "(" << i << " : " << positions[i] << ")" << ' ';
  cout << endl;  
}