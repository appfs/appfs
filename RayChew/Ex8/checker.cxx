#include "checker.h"

bool checker::isConnected(std::vector<Edge>& edges, int& startTerminal) { // cite: https://stackoverflow.com/questions/16411038/bfs-and-dfs-on-adjacency-list
  std::cout << "starting BFS..." << std::endl;
  int n = utils::get_Nodes(edges).back();
  n++;
  std::vector<std::vector<int>> graph = utils::build_adjList(n, edges);
  bool visited[n] = {false};
  int startNode = startTerminal;
  visited[startNode] = true;
  std::queue<std::pair<int,int>> queue;
  queue.push(std::make_pair(0,startNode));
  
  std::pair<int,int> node;
  int neighbour;
  while(!queue.empty()) {
    node = queue.front();
    queue.pop();
    std::vector<int> neighbours = graph[node.second];
    for (auto i=neighbours.begin(), end=neighbours.end(); i!=end ; i++) {
      neighbour = *i;
      //std::cout << "node: " << node.second << " neighbour: " << neighbour << " parent: " << node.first << std::endl;
      if(visited[neighbour] == false) {
	visited[neighbour] = true;
	queue.push(std::make_pair(node.second,neighbour));
      }
      else if(neighbour != node.first) {
	std::cout << "Solution is NOT a tree." << std::endl;
	return false;
	break;
      }
    }
  }
  std::cout << "Solution is a tree." << std::endl;
  return true;
}

bool checker::isFeasible(std::vector<Edge>& edges, std::vector<int>& primes) {
  std::vector<int> nodes = utils::get_Nodes(edges);
  if (std::includes(nodes.begin(), nodes.end(), primes.begin(), primes.end())) {
    std::cout << "Solution is feasible." << std::endl;
    return true;
  }
  else {
    std::cout << "Solution is NOT feasible." << std::endl;
    return false;
  }
}