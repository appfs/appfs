#include "utils.h"

using namespace std;

vector<int> utils::get_Nodes(vector<Edge>& edges) { // function to get all the nodes present in a collection of edges, without repetition.
  set<int> nodes;
  
  for (auto i = edges.begin(), end = edges.end(); i != end; i++){ // put the nodes into a set, which removes duplicates.
    nodes.insert(i->first);
    nodes.insert(i->second);
  }
  vector<int> nodesAsVector;
  copy(nodes.begin(), nodes.end(), back_inserter(nodesAsVector)); // convert set to vector.
  
  assert(nodes.size() <= 2*edges.size()); // the numbers of nodes in the graph cannot be less than the twice the size of the graph.
    
  return nodesAsVector;
}

pair<vector<Edge>,vector<int>> utils::get_EdgesWeights (int& n, ifstream& file) { // read file to get edges and weights.
  /* start get list of edges and weights */
  vector<Edge> Edges; // vector to store edges as pair<int,int>
  vector<int> Weights; // vector to store weights as integers.
  string str; // string to store line of graph file.
  
  while (getline(file,str)) { /// get graph line-by-line.
    int Vert1;
    int Vert2;
    int Weight;
    
    auto it = str.begin(); /// initializes iterator for qi::parse. 
    
    parse(it, str.end(), int_[([&Vert1](int i){Vert1 = i;})] >> qi::space >> int_[([&Vert2](int i){Vert2 = i;})] >> qi::space >> double_[([&Weight](int i){Weight = i;})]);  
    
    Edge edge = make_pair(Vert1, Vert2);  /// make edge-pair out of vertices.
    Edges.push_back(edge);
    Weights.push_back(Weight);
  }
  /* end get list of edges and weights */
  return make_pair(Edges,Weights);
}


vector<int> utils::gen_Primes(int& n) { // ref: https://stackoverflow.com/questions/5200879/printing-prime-numbers-from-1-through-100/
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
      primes.push_back(i); // push back to vector if value is prime.
    }
  }
  return primes;
}

vector<int> utils::get_PrimeDists(vector<int>& dists, vector<int>& primes) { // from a list of distances, get the distances of the prime nodes.
  int j = 0;
  vector<int> primeDists(primes.size());
  for(auto i=primes.begin(), end=primes.end(); i!=end; i++) { // for each prime,
    primeDists[j] = dists[*i]; // get the distance by index and put it into the primeDists list.
    j++;
  }
  return primeDists;
}

pair<int,int> utils::get_Min(vector<int>& primeDists, vector<int>& primes) { // If two prime vertices have the minimum weight, the one with the smaller index is chosen.
  vector<int>::iterator minDistAddr = min_element(primeDists.begin(), primeDists.end(),[](const int& i, const int& j) {return i > 0 && (j<=0 || i<j);}); // minimum weight larger than zero.
  int minDistIdx = distance(primeDists.begin(),minDistAddr); // get the index of the prime with the minimum distance > 0.
  int minDist = *minDistAddr; // and its distance.
  return make_pair(minDistIdx, minDist);
}
  
vector<vector<Vertex>> utils::build_adjList(int& n, vector<Edge>& edges, vector<int>& weights) { // build the adjacency list from list of edges and weights.
  vector<vector<Vertex>> adjList(n); // initialize adjList.
  int j = 0;
  for (auto i=edges.begin(), end=edges.end(); i!=end; i++) {
    int vert1 = i->first; // for each vertex,
    int vert2 = i->second;
    
    adjList[vert1].push_back(make_pair(vert2,weights[j])); // add it's neighbour and the weight from the vertex to the neighbour.
    adjList[vert2].push_back(make_pair(vert1,weights[j])); // do the same from the perspective of the neighbour.
    j++;
  }
  return adjList;
}

vector<vector<int>> utils::build_adjList(int& n,vector<Edge>& edges) { // overloaded function for adjacency list without weight. Used for building graph structure for the Steiner subgraph.
  vector<vector<int>> adjList(n);
  for (auto i=edges.begin(), end=edges.end(); i!=end; i++) {
    int vert1 = i->first;
    int vert2 = i->second;
    
    adjList[vert1].push_back(vert2);
    adjList[vert2].push_back(vert1);
  }
  return adjList;
}