/**
 * \file ex10.cxx
 * \author Ray Chew
 * \date 20 July 2017
 * \brief Steiner-Tree builder for undirected .gph graphs
 */

/**
 * \mainpage Ex10
 * 
 * \section Description
 * 
 * * compile: `g++ -std=c++14 utils.cxx checker.cxx boost_dijkstra.cxx m2alg.cxx ex8.cxx -o ex8 -lboost_timer -Wall`<br>
 * * OR: just use make.
 * * run: `./ex10 filepath/graph.gph [source node]`<br>
 */

#include "src/utils.h"
#include "src/checker.h"
#include "src/heap.h"
#include "src/dijkstra_alg.h"
//#include <stdlib.h> 

using namespace std;

int main(int argc, char* argv[]) {
  
  /* start getting command line arguments */
  int firstNTerminals = numeric_limits<int>::max(); // store the number of first N terminals to loop/check through.
  int nThreads = numeric_limits<int>::max(); // store the number of threads to use.
  int nThreadsUsed = nThreads; // to check if indeed threads were used.
  string fileName; // to store path/filename.
  
  // boost::program_options. Reference/Tutorial: http://www.boost.org/doc/libs/1_60_0/doc/html/program_options/tutorial.html
  po::options_description desc("Allowed options");
  desc.add_options()
      ("help", "This help message.")
      ("FILE", po::value<string>(&fileName), "Input file path and name.")
      ("firstNTerminals", po::value<int>(&firstNTerminals)->default_value(100),"First N terminals to check.")
      ("showEdges,s", po::value<string>()->implicit_value(""),"Show best Steiner-Graph edges.")
      ("THREADS", po::value<int>(&nThreads)->default_value(4),"Number of threads to use for parallelization.");
  
  po::positional_options_description p;
  p.add("FILE", 1);
  p.add("firstNTerminals", 2);
  
  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).
	    options(desc).positional(p).run(), vm);
  po::notify(vm);
    
  if (vm.count("help")) { // display help message and end program.
      cout << desc << endl;
      return 0;
  }
  
  if (!vm.count("FILE")) { // if no filename...
      cout << "No graph file." << endl;
      return -1; // return error.
  }
  /* start getting command line arguments */
  
  
  /* start get number of edges */
  ifstream file(fileName);  // read graph file.
  string str; /// read graph file line by line.
  
  getline(file, str);
  int n; /// store n as int for number of edges.
  
  auto it = str.begin();
  parse(it, str.end(), int_[([&n](int i){n = i;})] >> int_);
  n++;
  /* end get number of edges */
  
  
  /* start read edges and weights from file */
  cout << endl;
  cout << "Getting edges and weights from file..." << endl;
  pair<vector<Edge>,vector<int>> edgesWeights = utils::get_EdgesWeights(n, file);
  file.close();
  /* end read edges and weights from file */
  
  
  /* start generating primes and get number of primes */
  vector<int> primes = utils::gen_Primes(n);
  int primesSize = primes.size();
  
  if (firstNTerminals > primesSize) { // if the number of terminals to go through is larger than there are terminals, then use the number of terminals.
    firstNTerminals = primesSize;
  }
  /* end generating primes andn get number of primes */
  
  
  /* start building graph and initalising needed stuff for Steiner subgraph. */
  vector<Edge> bestSteinerEdges;
  int bestObjVal = numeric_limits<int>::max();
  int bestStartTerminal = numeric_limits<int>::max();
  
  cout << "Building graph..." << endl;
  vector<vector<Vertex>> originalAdjList = utils::build_adjList(n, edgesWeights.first, edgesWeights.second); // build the original adjacency list. Each thread will get a copy of this adjList.
  /* end building graph and initalising needed stuff for Steiner subgraph. */
  
  /* start calculating steiner trees. */
  cout << "Calculating steiner-trees..." << endl;
  
  boost::timer::cpu_timer timer; // start timer
  
  #pragma omp parallel for firstprivate(originalAdjList) num_threads(nThreads) // parallel!!! Aha!
  for (int i=0;i<firstNTerminals;i++) { // for the number of terminals to be looped over,
    vector<vector<Vertex>> adjList =  originalAdjList; // make a copy of the adjacency list.
    const int startTerminal = primes[i]; // get the terminal index that is the current start terminal.
    
    vector<Edge> subgraphEdges; // edge vector to store edges of the Steiner-subgraph.
    vector<int> subgraphWeights; // weight vector to store the weights of the edges of the Steiner-subgraph.
    vector<int> dists; // dist and parents to store the distances and parents/predecessors of the calculated shortest-paths from start node.
    vector<int> parents; 
    
    // steiner-tree heuristic looks through each terminal while building the steiner-subgraph. The first terminal is the start terminal.
    int currentPrime = startTerminal; // each terminal to be looped over is the "currentPrime".
    
    for(int j = 0; j<primesSize; j++) { // for each prime/terminal,
      vector<int> subgraphNodes = utils::get_Nodes(subgraphEdges); // get the current subgraph.
      myHeap Unvisited(n, currentPrime, subgraphNodes); // and initialise the priority queue for the dijkstra algorithm.
      
      pair<vector<int>,vector<int>> shortestDists = dijkstra_alg::alg(n, Unvisited, adjList, currentPrime); // calculate the shortest distances using dijkstra algorithm.
      dists = shortestDists.first; // get the distances of all the nodes from the current prime, 
      parents = shortestDists.second; // and their parents/predecessors.
      
      vector<int> primeDists = utils::get_PrimeDists(dists, primes); // from these distances, get those of the primes,
      pair<int,int> minPrimeIdxDist = utils::get_Min(primeDists,primes); // and then get the minimum of all the prime distances, and the corresponding index of the prime in the primes list.
      
      int minPrimeIdx = minPrimeIdxDist.first; // index of the prime in the primes list.
      int minPrime = primes[minPrimeIdx]; // the minimum prime-value itself.
      
      int currentWeight = numeric_limits<int>::max(); // initialise current weight of the edge under consideration, for the steiner-heuristic.
      int currentNode = minPrime; // start node for the steiner-heuristic is the current minimum prime-value.
      int pred = parents[currentNode]; // trace the next node on the edge by looking up the parents list.
      
      while(find(subgraphNodes.begin(),subgraphNodes.end(),currentNode)==subgraphNodes.end()){ // so long as the current node is not in the Steiner-subgraph...
	Edge edge = make_pair(pred,currentNode); // make an edge out of current node and its parent.
	
	int predIdx = dijkstra_alg::get_NeighbourIndex(currentNode, pred, adjList); // look up the index of the parent on the adjacency list from perspective of current node.
	int currentNodeIdx = dijkstra_alg::get_NeighbourIndex(pred, currentNode,adjList); // look up the index of the current node from the perspective of the parent.
	
	currentWeight = adjList[currentNode][predIdx].second; // get the weight of this edge.

	// add the edge and its weight to the steiner-subgraph.
	subgraphEdges.push_back(edge);
	subgraphWeights.push_back(currentWeight);
	
	// change edge weight on both directions (undirected graph)
	adjList[currentNode][predIdx].second = 0;
	adjList[pred][currentNodeIdx].second = 0;
	
	// update to the next edge.
	currentNode = pred;
	pred = parents[currentNode];
	
	// check if the edge has been traced to the start terminal, if yes, then break.
	if (pred == currentNode) {
	  break;
	}
      }
      currentPrime = minPrime; // in the next iteration, find shortest distances in graph from the terminal that was just added to the Steiner subgraph.
    }
    
    int objVal = 0;
    for_each(subgraphWeights.begin(), subgraphWeights.end(), [&] (int n) { // objective value is the sum of the edge weights.
      objVal += n;
    });
    
    if (omp_get_num_threads() != nThreadsUsed) { // check if a different number of threads than specified were used, in that case, openMP has issues...
      nThreadsUsed = omp_get_num_threads();
    }
    
    #pragma omp critical
    if(objVal<bestObjVal) { // update the best objective value if it is less than before.
      bestObjVal = objVal;
      bestSteinerEdges = subgraphEdges; // update the best Steiner subgraph as well,
      bestStartTerminal = startTerminal; // and the best StartTerminal that allowed this to happen...
    }
  }
  boost::timer::cpu_times times = timer.elapsed(); // once done, stop recording time.
  /* end calculating steiner trees. */
  
  
  /* start checking the best tree. */
  cout << "Checking steiner tree..." << endl; // see if graph is a tree and contains all terminals.
  checker::isConnected(bestSteinerEdges, bestStartTerminal);
  checker::isFeasible(bestSteinerEdges,primes);
  cout << "Number of threads used: " << nThreadsUsed << endl;
  cout << endl;
  /* end checking the best tree. */
  
  
  /* start printing values. */
  cout << "TLEN: " << bestObjVal << endl;
  
  if (vm.count("showEdges")) { // print the edges of the subgraph if -s is present.
    cout << "TREE: ";
    for (auto edge=bestSteinerEdges.begin(),end=bestSteinerEdges.end(); edge!=end; edge++) {
      cout << "("<<edge->first<<","<<edge->second<<") ";
    }
    cout << endl;
  }
  /// print CPU- and Wall-Time. 
  cout << "TIME: " << setprecision(4) << times.user / 1e9 << "s" << endl;
  cout << "WALL: " << setprecision(4) << times.wall / 1e9 << "s" << endl;
  cout << endl;
  /* end printing values. */
  
  return 0;
}
