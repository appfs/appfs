/** 
 *  @file    checker.cxx
 *  @author  Ray Chew
 *  @date    20 July 2017
 *  
 *  @brief checker namespace containing the functions to check solutions of steiner subgraph.
 *
 */

#include "checker.h"

using namespace std;

/** 
 *   @brief  Checks if graph is a tree (acyclic). Return true if is tree and false otherwise.
 *  
 *   @param  edges is a vector<Edge> of edges for a graph.
 *   @param  startTerminal is a vertex index on the graph.
 *   @return bool
 */  
bool checker::isTree(vector<Edge>& edges, const int& startTerminal) {
  // ref: https://stackoverflow.com/questions/16411038/bfs-and-dfs-on-adjacency-list
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
    // get this start edge and search the neighbours of the target node...
    edge = queue.front();
    queue.pop(); 
    vector<int> neighbours = graph[edge.second];
    
    // for each neighbour of the target node, if the neighbour has not been visited, then mark it as visited.
    for (auto i=neighbours.begin(), end=neighbours.end(); i!=end ; i++) {
      neighbour = *i;
      if(visited[neighbour] == false) {
	visited[neighbour] = true;
	// then add this edge of "(target node, neighbour node)" onto the queue to be checked next.
	queue.push(make_pair(edge.second,neighbour));
      }
      // otherwise, neighbour has been visited. In this case, it MUST be the source node. If not, it is not a tree.
      else if(neighbour != edge.first) {
	return false;
	break;
      }
    }
  }
  // otherwise, if all the checks pass, the graph is a tree.
  return true;
}

/** 
 *   @brief  Checks if graph contains all terminals. Return true if is all terminals are present and false otherwise.
 *  
 *   @param  edges is a vector<Edge> of edges for a graph.
 *   @param  primes is the list of terminals for the graph.
 *   @return bool
 */  
bool checker::isFeasible(vector<Edge>& edges, vector<int>& primes) {
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