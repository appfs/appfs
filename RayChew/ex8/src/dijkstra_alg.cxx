#include "dijkstra_alg.h"

using namespace std;

/** 
 *   @brief  Main Dijkstra algorithm. Returns vectors containing the distances and the parents of each of the node on a graph.
 *  
 *   @param  n is an int of the number of nodes in the graph.
 *   @param  Unvisited is a binary heap object of the class myHeap.
 *   @param  adjList is a vector<vector<Vertex>> of adjacency list with (index,weight).
 *   @param  startTerminal is the index of the starting node for the Dijkstra algorithm.
 *   @return pair<vector<int>,vector<int>>
 */  
pair<vector<int>,vector<int>> dijkstra_alg::alg (int& n, myHeap& Unvisited, vector<vector<Vertex>>& adjList, const int& startTerminal, vector<bool>& inSubgraph) { // the main Dijkstra algorithm.
  // initialize vectors to store the final calculate weights and parents(predecessors) for each node.
  vector<int> finalWeights(n, numeric_limits<int>::max());
  vector<int> finalParents(n);
  finalWeights[startTerminal] = 0; // the start terminal has 0 weight and the parent as itself.
  finalParents[startTerminal] = startTerminal;
  
  for (int i=0,end=inSubgraph.size(); i<end; i++) { // all nodes in subgraph should have zero weight.
    if (inSubgraph[i] == true) {
      finalWeights[i] = 0;
      finalParents[i] = startTerminal;
    }
  }
  
  /* start calculating and updating distances */
  while (Unvisited.size() > 0) { // while the unvisited priority queue is not empty...
    Vertex minPair = Unvisited.get_min(); // get the node with the minimum weight on the queue
    int minIdx = minPair.second; // get its index,
    int minDist = minPair.first; // and get its distance to the start terminal.
    assert(minDist != numeric_limits<int>::max()); // since all non-subgraph nodes are initialised with infinite distance, each minimum distance on the heap must be "updated" first before it is accessed.
    
    vector<pair<int,int>> *neighboursOfMinIdx = &adjList[minIdx]; // then get the neighbours of this "minimum" node from the adjacency list.
    int neighboursOfMinIdxSize = neighboursOfMinIdx->size();
    
    for(int j=0; j<neighboursOfMinIdxSize; j++) { // for each neighbour,
      int neighbour = (*neighboursOfMinIdx)[j].first; // get the node-index of the neighbour on the graph
      int dist = (*neighboursOfMinIdx)[j].second; // and its distance to the "minimum" node.
      
      int newDist = minDist + dist; // find the new distance of this neighbouring node to the start terminal.
      int currentDist = finalWeights[neighbour]; // get the old distance of this neighbouring node to the start terminal.
      
      assert(newDist >= 0); // distances should be larger than zero. Negative distances are most likely due to int overflow.
      assert(newDist != numeric_limits<int>::max()); // want to make sure that all distances were "touched".
      
      if (newDist < currentDist) { // if the new distance is smaller,

	int *toBeInserted = Unvisited.get_neighbourPosition(neighbour); // get the index of the neighbour node on the priority queue,
	if (*toBeInserted != numeric_limits<int>::min()) { // and if the neighbour is still in the queue,
	  Unvisited.update_weight(toBeInserted,newDist); // update its weight (distance to the start terminal) to be the newly calculated distance.
	}

	finalWeights[neighbour] = newDist; // then for this neighbouring node, update the final distance,
	finalParents[neighbour] = minIdx; // and note that its parent is the current "minimum" node.
      }
    }
    Unvisited.pop_top(); // remove the minimum value (at the top) of the priority queue.
  }
  return make_pair(finalWeights,finalParents);
  /* end calculating and updating distances */
}
