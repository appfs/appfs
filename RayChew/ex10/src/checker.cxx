#include "checker.h"

using namespace std;


/** 
 *   @brief  Checks if graph is a tree (acyclic). Return true if is tree and false otherwise.
 *  
 *   @param  edges is a vector<pair<int,int>> of edges for a graph.
 *   @param  startTerminal is a vertex index on the graph.
 *   @return bool
 */  
bool checker::isTree(vector<Edge>& edges, const int& startTerminal) { // function to check if graph is a tree.
  // reference usd: https://stackoverflow.com/questions/16411038/bfs-and-dfs-on-adjacency-list
  int n = utils::get_Nodes(edges).back(); 
  n++;
  vector<vector<int>> graph = utils::build_adjList(n, edges);
  vector<bool> visited(n,false); // all node are by default unvisited.
  int startNode = startTerminal;
  visited[startNode] = true;
  queue<Edge> queue; // initalise queue of edges
  queue.push(make_pair(0,startNode)); // first edge has 0 as source, and startNode as the target.
  
  Edge edge;
  int neighbour;
  while(!queue.empty()) {
    edge = queue.front();
    queue.pop(); // get this start edge and...
    vector<int> neighbours = graph[edge.second]; // search the neighbours of the target node...
    for (auto i=neighbours.begin(), end=neighbours.end(); i!=end ; i++) { // for each neighbour of the target node,
      neighbour = *i;
      if(visited[neighbour] == false) { // if the neighbour has not been visited,
	visited[neighbour] = true; // then mark it as visited.
	queue.push(make_pair(edge.second,neighbour)); // then add this edge of "(target node, neighbour node)" onto the queue to be checked next.
      }
      else if(neighbour != edge.first) { // otherwise, neighbour has been visited. In this case, it MUST be the source node. If not, it is not a tree.
	//cout << "Solution is NOT a tree." << endl;
	return false;
	break; // so, break and return false.
      }
    }
  }
  //cout << "Solution is a tree." << endl;
  return true; // otherwise, if all the checks pass, the graph is a tree.
}

/** 
 *   @brief  Checks if graph contains all terminals. Return true if is all terminals are present and false otherwise.
 *  
 *   @param  edges is a vector<pair<int,int>> of edges for a graph.
 *   @param  primes is the list of terminals for the graph.
 *   @return bool
 */  
bool checker::isFeasible(vector<Edge>& edges, vector<int>& primes) { // function to check if all the primes/terminals are present in a list of edges.
  vector<int> nodes = utils::get_Nodes(edges);
  if (includes(nodes.begin(), nodes.end(), primes.begin(), primes.end())) {
    cout << "Solution is feasible." << endl;
    return true;
  }
  else {
    cout << "Solution is NOT feasible." << endl;
    return false;
  }
}