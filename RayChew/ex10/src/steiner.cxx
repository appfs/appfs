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
  // initialize vectors to store the final calculate weights and parents(predecessors) for each node.
  vector<bool> inSubgraph(n,false); // boolean vector where node numbers in steiner subgraph are true, else false.
  inSubgraph[startTerminal] = true; // the start terminal should always be in subgraph...
  vector<Edge> subgraphEdges; // edge vector to store edges of the Steiner-subgraph.
  vector<int> subgraphWeights; // weight vector to store the weights of the edges of the Steiner-subgraph.
  vector<int> dists(n, numeric_limits<int>::max());
  vector<int> parents(n);
  dists[startTerminal] = 0; // the start terminal has 0 weight
  //parents[startTerminal] = startTerminal;
  
  /* start dijkstra part */
  while (Unvisited.size() > 0) { // while the unvisited priority queue is not empty...
    Vertex minPair = Unvisited.get_min(); // get the node (index,dist) with the minimum distance on the queue.
    Unvisited.pop_top(); // remove the minimum value (at the top) of the priority queue.
    int minIdx = minPair.second;
    int minDist = minPair.first;
    assert((minDist == numeric_limits<int>::max()) || (minIdx > 0)); // since all non-subgraph nodes are initialised with infinite distance, each minimum distance on the heap must be "updated" first before it is accessed. Only node with infinity distance should be zero.
    
    vector<pair<int,int>> *neighboursOfMinIdx = &adjList[minIdx]; // then get the neighbours of this "minimum" node from the adjacency list.
    
    for(auto currentNeighbour = neighboursOfMinIdx->begin(), end = neighboursOfMinIdx->end(); currentNeighbour!=end; currentNeighbour++) { // for each neighbour,
      int neighbour = currentNeighbour->first; // get the node-index of the neighbour on the graph
      int dist = currentNeighbour->second; // and its distance to the "minimum" node.
      
      //cout << startTerminal << "," << minIdx << "," << minDist << "," << neighbour << "," << dist << endl;
      int newDist = minDist + dist; // find the new distance of this neighbouring node to the start terminal.
      assert(newDist >= 0); // distances should be larger than zero. Negative distances are most likely due to int overflow.
      assert(newDist != numeric_limits<int>::max()); // want to make sure that all distances were "touched".
      
      if (newDist < dists[neighbour]) { // if the new distance is smaller,
	Unvisited.update_weight(neighbour,newDist); // update its distance to the start terminal to be the newly calculated distance.
	
	dists[neighbour] = newDist; // then for this neighbouring node, update the final distance,
	parents[neighbour] = minIdx; // and note that its parent is the current "minimum" node.
      }  
    }
    
    /* start steiner heuristic */
    // add each terminal and the edges connecting it to the steiner subgraph onto the subgraph and insert the nodes back to the priority queue.
    if (isPrimes[minIdx]) { // if the current node is a terminal/prime,
      int currentNode = minIdx; // then get its index and parent.
      int pred = parents[currentNode];
      int currentWeight;
      while (!inSubgraph[currentNode]) { // So long as the current node is not in the Steiner-subgraph...
	Edge edge = make_pair(pred,currentNode); // make an edge out of current node and its parent.
	currentWeight = dists[currentNode] - dists[pred]; // distance of the current node to its parent (the edge weight) is the shortest distance to the start terminal of the current node, minus that of the parent node.
// 	for (auto it =adjList[currentNode].begin(), end=adjList[currentNode].end(); it!=end; it++) {
// 	  if (it->first == pred) {
// 	    currentWeight = it->second;
// 	  }
// 	}
	
	// something is very wrong if the parent of the current node is a prime, is not yet in the steiner subgraph, and has non-zero weight. Assert this.
	assert(!((inSubgraph[pred]==false) && (isPrimes[pred]) && (currentWeight!=0)));
	
	// add the edge and its weight to the steiner-subgraph.
	subgraphEdges.push_back(edge);
	subgraphWeights.push_back(currentWeight);
	inSubgraph[currentNode] = true; // add current node to reflect that it is in the steiner-subgraph.
	dists[currentNode] = 0; // update the distances of the dijkstra algorithm to zero since the current node now belongs to the subgraph.
	
	//int oo= 0;
	Unvisited.insert(make_pair(0,currentNode)); // insert the current node back into the priority queue.
	//Unvisited.update_weight(currentNode,oo);
	
	// update to the next edge.
	currentNode = pred;
	pred = parents[currentNode];
	
	// check if the edge has been traced to the start terminal, if yes, then break.
// 	if (pred == currentNode) {
// 	  break;
// 	}
      }
    }
    /* end steiner heuristic */
  }
  return make_pair(subgraphEdges,subgraphWeights);
}
