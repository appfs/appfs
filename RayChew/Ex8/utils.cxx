#include "utils.h"

//using namespace std;

std::vector<int> utils::get_Nodes(std::vector<Edge>& edges) {
  std::set<int> nodes;
  for (auto i = edges.begin(), end = edges.end(); i != end; i++){
    nodes.insert(i->first);
    nodes.insert(i->second);
  }
  std::vector<int> nodesAsVector;
  std::copy(nodes.begin(), nodes.end(), std::back_inserter(nodesAsVector));
  return nodesAsVector;
}

std::pair<std::vector<Edge>,std::vector<int>> utils::get_EdgesWeights (int& n, std::ifstream& file) {
  /* start get list of edges and weights */
  std::cout << "getting edges and weights from file..." << std::endl;
  std::vector<Edge> Edges;
  std::vector<int> Weights; // vector to weights as integers.
  std::string str; // string to store line of graph file.
  
  while (getline(file,str)) { /// get graph line-by-line.
    int Vert1;
    int Vert2;
    int Weight;
    
    auto it = str.begin(); /// initializes iterator for qi::parse. 
    
    parse(it, str.end(), int_[([&Vert1](int i){Vert1 = i;})] >> qi::space >> int_[([&Vert2](int i){Vert2 = i;})] >> qi::space >> double_[([&Weight](int i){Weight = i;})]);  
    
    Edge edge = std::make_pair(Vert1, Vert2);  /// make edge-pair out of vertices.
    Edges.push_back(edge);
    if (Weight == 0){
      Weight = std::numeric_limits<int>::max();
    }
    Weights.push_back(Weight);
  }
  /* end get list of edges and weights */
  return std::make_pair(Edges,Weights);
}


std::vector<int> utils::gen_Primes(int& n) { // cite: https://stackoverflow.com/questions/5200879/printing-prime-numbers-from-1-through-100/
  std::vector<int> primes;
  primes.push_back(2);
  for(int i=3; i < n; i=i+2) {
    bool isPrime=true;
    for(auto j = primes.begin(), end = primes.end();j != end && (*j)*(*j) <= i;j++) {
      if(i % *j == 0) {
	isPrime=false;
	break;
      }
    }
    if(isPrime){
      primes.push_back(i);
    }
  }
  return primes;
}

std::vector<int> utils::get_PrimeDists(std::vector<int>& dists, std::vector<int>& primes) {
  int j = 0;
  std::vector<int> primeDists(primes.size());
  for(auto i=primes.begin(), end=primes.end(); i!=end; i++) {
    primeDists[j] = dists[*i];
    j++;
    //std::cout << primes[i] << " " << primeDists[i] << std::endl;
  }
  return primeDists;
}

std::pair<int,int> utils::get_Min(std::vector<int>& primeDists, std::vector<int>& primes) { // If two prime vertices have the minimum weight, the one with the smaller index is chosen.
  std::vector<int>::iterator minDistAddr = std::min_element(primeDists.begin(), primeDists.end(),[](const int& i, const int& j) {return i > 0 && (j<=0 || i<j);}); // minimum weight larger than zero.
  int minDistIdx = std::distance(primeDists.begin(),minDistAddr);
  //int minPrime = primes[minDistIdx];
  int minDist = *minDistAddr;
  return std::make_pair(minDistIdx, minDist);
}
  
std::vector<std::vector<vertex>> utils::build_adjList(int& n, std::vector<Edge>& edges, std::vector<int>& weights) {
  std::vector<std::vector<vertex>> adjList(n); // initialize adjList such that each row starts from index 1.
  //for (int j=0; j<adjList.size(); j++){
  //  adjList[j].push_back(std::make_pair(0,0));
  //}
  int j = 0;
  for (auto i=edges.begin(), end=edges.end(); i!=end; i++) {
    int vert1 = i->first;
    int vert2 = i->second;
    
    adjList[vert1].push_back(std::make_pair(vert2,weights[j]));
    adjList[vert2].push_back(std::make_pair(vert1,weights[j]));
    j++;
  }
  return adjList;
}

std::vector<std::vector<int>> utils::build_adjList(int& n,std::vector<Edge>& edges) {
  std::vector<std::vector<int>> adjList(n);
  for (auto i=edges.begin(), end=edges.end(); i!=end; i++) {
    int vert1 = i->first;
    int vert2 = i->second;
    
    adjList[vert1].push_back(vert2);
    adjList[vert2].push_back(vert1);
  }
  return adjList;
}