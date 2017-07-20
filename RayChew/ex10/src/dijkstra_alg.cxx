#include "dijkstra_alg.h"

using namespace std;

pair<vector<int>,vector<int>> dijkstra_alg::alg (int& n, myHeap& Unvisited, vector<vector<Vertex>>& adjList, int& startTerminal) { // the main Dijkstra algorithm.
  // initialize vectors to store the final calculate weights and parents(predecessors) for each node.
  vector<int> finalWeights(n, numeric_limits<int>::max());
  vector<int> finalParents(n);
  finalWeights[startTerminal] = 0; // the start terminal has 0 weight and the parent as itself.
  finalParents[startTerminal] = startTerminal;
  
  /* start calculating and updating distances */
  while(Unvisited.size()>0) { // while the unvisited priority queue is not empty...
    Vertex minPair = Unvisited.get_min(); // get the node with the minimum weight on the queue
    int minIdx = minPair.second; // get its index,
    int minDist = minPair.first; // and get its distance to the start terminal.
    
    vector<pair<int,int>> *neighboursOfMinIdx = &adjList[minIdx]; // then get the neighbours of this "minimum" node from the adjacency list.
    int neighboursOfMinIdxSize = neighboursOfMinIdx->size();
    
    for(int j=0; j<neighboursOfMinIdxSize; j++) { // for each neighbour,
      int neighbour = (*neighboursOfMinIdx)[j].first; // get the node-index of the neighbour on the graph
      int dist = (*neighboursOfMinIdx)[j].second; // and its distance to the "minimum" node.
      
      int newDist = minDist + dist; // find the new distance of this neighbouring node to the start terminal.
      int currentDist = finalWeights[neighbour]; // get the old distance of this neighbouring node to the start terminal.
      
      if (newDist < currentDist) { // if the new distance is smaller,

	int toBeInserted = Unvisited.get_neighbourPosition(neighbour); // get the index of the neighbour node on the priority queue,
	if (toBeInserted != numeric_limits<int>::min()) { // and if the neighbour is still in the queue,
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

int dijkstra_alg::get_NeighbourIndex(int& currentNode, int& neighbour, vector<vector<Vertex>>& adjList) {
  vector<Vertex> neighboursOfCurrentNode = adjList[currentNode]; // get list of neighbours of the current input node.
  int possibleNeighbourIdx = 0;
  int weight = numeric_limits<int>::max();
  for (int i=0, end=neighboursOfCurrentNode.size(); i<end; i++){ // for each neighbour,
    if ((neighboursOfCurrentNode[i].first == neighbour) && (neighboursOfCurrentNode[i].second < weight)) { // find the edge with the least weight (for directed graphs or graphs with overlapping edges)
      possibleNeighbourIdx = i;
      weight = neighboursOfCurrentNode[i].second;
    }
  }
  return (possibleNeighbourIdx); // find the position/index of the returned iterator pos.
}
