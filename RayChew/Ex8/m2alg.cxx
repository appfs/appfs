#include "m2alg.h"

bimap m2alg::build_unvisited(int& n, int& startTerminal, std::vector<std::pair<int,int>>& edges) {
  /* start initializing unvisited set */
  bimap Unvisited; /// define unvisited set as a boost::bimap container.
  std::vector<int> nodes = utils::get_Nodes(edges);
  for(int i=1; i<n; i++) {
    Unvisited.left.insert(bimap::left_value_type(i, std::numeric_limits<int>::max()));
  }
  
  bimap::left_iterator itr = Unvisited.left.find(startTerminal);
  Unvisited.left.replace_data(itr, 0);
  
  if (!nodes.empty()) {
    for(auto j=nodes.begin(), end=nodes.end(); j!=end; j++) {
      bimap::left_iterator itr = Unvisited.left.find(*j);
      Unvisited.left.replace_data(itr, 0);    
    }
  }

  return Unvisited;
  /* end initializing unvisited set */
}

std::pair<std::vector<int>,std::vector<int>> m2alg::m2Algorithm (int& n, bimap& Unvisited, std::vector<std::vector<std::pair<int,int>>>& adjList, int& startTerminal) {
  std::vector<int> finalWeights(n, std::numeric_limits<int>::max());
  std::vector<int> finalParents(n);

  finalWeights[startTerminal] = 0;
  finalParents[startTerminal] = startTerminal;
  
  /* start calculating and updating distances */
  //std::cout << "calculating distances..." << std::endl;
  while(Unvisited.size()>0) {
    auto minPair = Unvisited.right.begin();
    int minIdx = minPair->second;
    int minDist = minPair->first;
    
    signed int adjListSize = adjList[minIdx].size();
    
    for(int j=0; j<adjListSize; j++) {
      int neighbour = adjList[minIdx][j].first;
      int dist = adjList[minIdx][j].second;
      int newDist = minDist + dist;
      int nPWeight = finalWeights[neighbour];
      
      if (newDist < nPWeight) {
	auto toBeReplaced = Unvisited.left.find(neighbour);
	if (toBeReplaced != Unvisited.left.end()){
	  Unvisited.left.modify_data(toBeReplaced, bm::_data=newDist);
	}
	finalWeights[neighbour] = newDist;
	finalParents[neighbour] = minIdx;
      }
    }
    Unvisited.left.erase(minIdx);
  }
  return std::make_pair(finalWeights,finalParents);
  /* end calculating and updating distances */
}

int m2alg::get_NeighbourIndex(int& currentNode, int& neighbour, std::vector<std::vector<vertex>>& adjList) {
  int j = 0;
  for (auto i=adjList[currentNode].begin(), end=adjList[currentNode].end(); i!=end; i++) {
    if (i->first==neighbour) {
      break;
    }
    j++;
  }
  return j;
}
