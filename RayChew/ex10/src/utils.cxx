#include "utils.h"

using namespace std;
/** 
 *   @brief  Given a collection of edges, return a vector<int> of nodes that are present in the edges without repetition.
 *  
 *   @param  edges is a vector<Edge> that contains the list of edges for which their nodes should be extracted.
 *   @return vector<int>
 */  
vector<int> utils::get_Nodes(vector<Edge>& edges) {
  set<int> nodes;
  
  for (auto i = edges.begin(), end = edges.end(); i != end; i++){ // use set to remove duplicates
    nodes.insert(i->first);
    nodes.insert(i->second);
  }
  vector<int> nodesAsVector;
  copy(nodes.begin(), nodes.end(), back_inserter(nodesAsVector)); // convert set to vector.
  
  assert(nodes.size() <= 2*edges.size()); // the numbers of nodes in the graph cannot be less than the twice the size of the graph.
  
  return nodesAsVector;
}


/** 
 *   @brief  Given a graph file, read it and return a pair<vector<Edge>,vector<int>> of edges and their respective weights.
 *  
 *   @param  n is an int that is the number of nodes in the graph file.
 *   @param  file is an ifstream that is the address to a opened graph file.
 *   @return pair<vector<Edge>,vector<int>>
 */  
pair<vector<Edge>,vector<int>> utils::get_EdgesWeights (int& n, ifstream& file) {
  vector<Edge> Edges; // vector to store edges as pair<int,int>
  vector<int> Weights; // vector to store weights as integers.
  string str; // string to store line of graph file.
  
  while (getline(file,str)) { // get graph line-by-line.
    int Vert1;
    int Vert2;
    int Weight;
    
    auto it = str.begin();
    
    parse(it, str.end(), int_[([&Vert1](int i){Vert1 = i;})] >> qi::space >> int_[([&Vert2](int i){Vert2 = i;})] >> qi::space >> double_[([&Weight](int i){Weight = i;})]);  
    
    Edge edge = make_pair(Vert1, Vert2);  // make edge-pair out of vertices.
    Edges.push_back(edge);
    Weights.push_back(Weight);
  }
  return make_pair(Edges,Weights);
}

/** 
 *   @brief  Given the number of nodes in a graph, generate the primes that are contained within this limit, and return them as vector<int>.
 *  
 *   @param  n is an int that is the number of nodes in the graph file.
 *   @return vector<int>
 */  
vector<int> utils::gen_Primes(int& n) {
  // ref: https://stackoverflow.com/questions/5200879/printing-prime-numbers-from-1-through-100/
  vector<int> primes; // vector to store primes numbers
  primes.push_back(2); // 2 is a prime. add this.
  for(int i=3; i < n; i=i+2) { // then for all odd numbers,
    bool isPrime=true;
    for(auto j = primes.begin(), end = primes.end();j != end && (*j)*(*j) <= i;j++) { // look in the range where the prime must be less than square-root of the divider.
      if(i % *j == 0) { // if divisible, then not prime
	isPrime=false;
	break;
      }
    }
    if(isPrime){
      primes.push_back(i); // if prime, push back.
    }
  }
  return primes;
}

/** 
 *   @brief  Given the number of nodes in a graph and the list of primes present in the graph, return a boolean vector where prime indices are indicated by true, and false otherwise.
 *  
 *   @param  n is an int that is the number of nodes in the graph file.
 *   @param  primes that is a vector<int> of the primes present in the graph.
 *   @return vector<bool>
 */
vector<bool> utils::isPrime(int& n, vector<int>& primes) {
  vector<bool> isPrimes(n,false);
  for (auto prime=primes.begin(),end=primes.end(); prime!=end; prime++) {
    isPrimes[*prime]=true;
  }
  return isPrimes;
}

/** 
 *   @brief  Build an adjacency list of a weighted graph given their edges and weights. Return this adjacency list as a vector<vector<Vertex>>.
 *  
 *   @param  n is an int that is the number of nodes in the graph file.
 *   @param  edges is a vector<Edge> that contains the list of edges of a graph.
 *   @param  weights is an vector<int> that contains the weights of the respective wedges.
 *   @return vector<vector<Vertex>>
 */  
vector<vector<Vertex>> utils::build_adjList(int& n, vector<Edge>& edges, vector<int>& weights) {
  vector<vector<Vertex>> adjList(n); // initialize adjList.
  int j = 0;
  for (auto i=edges.begin(), end=edges.end(); i!=end; i++) {
    int vert1 = i->first;
    int vert2 = i->second;
    
    adjList[vert1].push_back(make_pair(vert2,weights[j])); // for each vertex, add it's neighbour and the weight from the vertex to the neighbour.
    adjList[vert2].push_back(make_pair(vert1,weights[j])); // do the same from the perspective of the neighbour.
    j++;
  }
  return adjList;
}

/** 
 *   @brief  Build an adjacency list of an unweighted graph given their edges. Return this adjacency list as a vector<vector<int>>.
 *  
 *   @param  n is an int that is the number of nodes in the graph file.
 *   @param  edges is a vector<Edge> that contains the list of edges of a graph.
 *   @return vector<vector<int>>
 */  
vector<vector<int>> utils::build_adjList(int& n,vector<Edge>& edges) {
  // overloaded function for adjacency list without weight. Used for building graph structure for the Steiner subgraph in the checker.
  vector<vector<int>> adjList(n);
  for (auto i=edges.begin(), end=edges.end(); i!=end; i++) {
    int vert1 = i->first;
    int vert2 = i->second;
    
    adjList[vert1].push_back(vert2);
    adjList[vert2].push_back(vert1);
  }
  return adjList;
}