#include "dijkstra_alg.h"

std::pair<std::vector<int>,std::vector<int>> dijkstra_alg::alg (int& n, myHeap& Unvisited, std::vector<std::vector<vertex>>& adjList, int& startTerminal, std::vector<Edge>& edges) {
  std::vector<int> finalWeights(n, std::numeric_limits<int>::max());
  std::vector<int> finalParents(n);
  finalWeights[startTerminal] = 0;
  finalParents[startTerminal] = startTerminal;
  
  /* start calculating and updating distances */
  //std::cout << "calculating distances..." << std::endl;
  while(Unvisited.size()>0) {
    vertex minPair = Unvisited.get_min();
    int minIdx = minPair.second;
    int minDist = minPair.first;
    signed int adjListSize = adjList[minIdx].size();
    
    //#pragma omp parallel default(none) shared(finalParents,finalWeights,Unvisited,adjList,minPair) private(neighbour,dist,newDist,nPWeight,bm::_data) firstprivate(minIdx,minDist,adjListSize)
    //#pragma omp for
    for(int j=0; j<adjListSize; j++) {
      int neighbour = adjList[minIdx][j].first;
      int dist = adjList[minIdx][j].second;
      
      int newDist = minDist + dist;
      int nPWeight = finalWeights[neighbour];
      
      if (newDist < nPWeight) {
	int toBeInserted = Unvisited.get_neighbourPosition(neighbour);
	if (toBeInserted != std::numeric_limits<int>::min()){
	  Unvisited.update_weight(toBeInserted,newDist);
	}
	finalWeights[neighbour] = newDist;
	finalParents[neighbour] = minIdx;
      }
    }
    Unvisited.pop_top();
  }
  return std::make_pair(finalWeights,finalParents);
  /* end calculating and updating distances */
}

int dijkstra_alg::get_NeighbourIndex(int& currentNode, int& neighbour, std::vector<std::vector<vertex>>& adjList) {
  int neighbourIdx = 0;
  for (auto i=adjList[currentNode].begin(), end=adjList[currentNode].end(); i!=end; i++) {
    if (i->first==neighbour) {
      break;
    }
    neighbourIdx++;
  }
  return neighbourIdx;
}
