#include "steiner.h"

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
pair<vector<Edge>,vector<int>> steiner::alg (int& n, myHeap& Unvisited, vector<vector<Vertex>>& adjList, const int& startTerminal, vector<bool>& isPrimes) { // the main Dijkstra algorithm.
  vector<bool> inSubgraph(n,false); // boolean vector where node numbers in steiner subgraph are true, else false.
  inSubgraph[startTerminal] = true; // the start terminal should always be in subgraph...
  
  // vector to store edges and weights of the Steiner-subgraph.
  vector<Edge> subgraphEdges;
  vector<int> subgraphWeights;
  
  // initialize vectors to store the final calculate weights and parents(predecessors) for each node.
  vector<int> dists(n, numeric_limits<int>::max());
  vector<int> parents(n);
  
  dists[startTerminal] = 0; // the start terminal has 0 distance to itself.
  
  /* start dijkstra part */
  while (Unvisited.size() > 1) { // while the unvisited priority queue is not empty...
    Vertex minPair = Unvisited.get_min(); // get the node (index,dist) with the minimum distance on the queue.
    Unvisited.pop_top(); // remove the minimum value (at the top) of the priority queue.
    int minIdx = minPair.second;
    int minDist = minPair.first;

    // since all non-subgraph nodes are initialised with infinite distance, each minimum distance on the heap must be "updated" first before it is accessed. Only node with infinity distance should be zero.
    assert((minDist == numeric_limits<int>::max()) || (minIdx > 0)); 
    
    vector<pair<int,int>> *neighboursOfMinIdx = &adjList[minIdx]; // then get the neighbours of this "minimum" node from the adjacency list.
    
    for(auto currentNeighbour = neighboursOfMinIdx->begin(), end = neighboursOfMinIdx->end(); currentNeighbour!=end; currentNeighbour++) {
      int neighbour = currentNeighbour->first; // get the node index and edge weight of each neighbour.
      int dist = currentNeighbour->second;
      
      int newDist = minDist + dist;
      assert(newDist >= 0); // distances should be larger than zero. Negative distances are most likely due to int overflow.
      assert(newDist != numeric_limits<int>::max()); // want to make sure that all distances were "touched".
      
      if (newDist < dists[neighbour]) {
	Unvisited.update_weight(neighbour,newDist); // update its distance to the start terminal to be the newly calculated distance.
	
	dists[neighbour] = newDist; // then for this neighbouring node, update the final distance and its parent/predecessor.
	parents[neighbour] = minIdx;
      }  
    }

    /* start steiner heuristic */
    // add each terminal and the edges connecting it to the steiner subgraph onto the subgraph and insert the nodes back to the priority queue.
    if ((isPrimes[minIdx]) && (minIdx != startTerminal)) { // if the current node is a terminal/prime, and is not the start terminal,
      int currentNode = minIdx;
      int pred = parents[currentNode];
      int currentWeight;
      
      while (!inSubgraph[currentNode]) { // So long as the current node is not in the Steiner-subgraph...
	Edge edge = make_pair(pred,currentNode); // make an edge out of current node and its parent.
	currentWeight = dists[currentNode] - dists[pred]; // distance of the current node to its parent (the edge weight) is the shortest distance to the start terminal of the current node, minus that of the parent node.
	
	// something is very wrong if the parent of the current node is a prime, is not yet in the steiner subgraph, and has non-zero weight. Assert this.
	assert(!((inSubgraph[pred]==false) && (isPrimes[pred]) && (currentWeight!=0)));
	
	// add the edge and its weight to the steiner-subgraph.
	subgraphEdges.push_back(edge);
	subgraphWeights.push_back(currentWeight);
	inSubgraph[currentNode] = true; // add current node to reflect that it is in the steiner-subgraph.
	dists[currentNode] = 0; // update the distances of the dijkstra algorithm to zero since the current node now belongs to the subgraph.
	
	Unvisited.insert(make_pair(0,currentNode)); // insert the current node back into the priority queue.
	
	// update to the next edge.
	currentNode = pred;
	pred = parents[currentNode];
      }
    }
    /* end steiner heuristic */
  }
  /* end dijkstra part */
  return make_pair(subgraphEdges,subgraphWeights);
}
