/**
 * \file ex10.cxx
 * \author Ray Chew
 * \date 20 July 2017
 * \brief Steiner-Tree builder for .gph graphs
 */

/**
 * \mainpage Ex10
 * 
 * \section Description
 * 
 * Heuristics that returns a Steiner Tree for a given graph with the prime-indexed nodes as terminals.
 * The Steiner Tree heuristics is based on that by H. Takahashi and A. Matsuyama, and the Dijkstra algorithm to find the shortest distances to a given terminal.
 * The Dijkstra function is dijkstra_alg in the dijkstra namespace, and the Steiner heuristic is below.
 * The Dijkstra priority queue uses a binary heap structure as an instance of the myHeap class.
 * Finally, the Steiner Tree is checked if it is a tree using the isTree function, and if it contains all the terminals using the isFeasible function. Both these functions are in the checker namespace.
 * The objective value of the Steiner Tree and its edges (if the -s flag is specified) will be printed.
 * 
 * * Compile using make. Specify at compile time THREADS=XX and FILE=YY for number of threads to be used and graph file to read.<br>
 * * run: `./ex10 filepath/graph.gph [first N terminals] [-s]`<br>
 */

#include "src/utils.h"
#include "src/checker.h"
#include "src/heap.h"
#include "src/dijkstra_alg.h"

using namespace std;

int main(int argc, char* argv[]) {
  
  /* start getting command line arguments */
  int firstNTerminals = numeric_limits<int>::max(); // store the number of first N terminals to loop/check through.
  string fileName; // store path/filename.
  
  // boost::program_options. Reference/Tutorial: http://www.boost.org/doc/libs/1_60_0/doc/html/program_options/tutorial.html
  po::options_description desc("Allowed options"); // define available options.
  desc.add_options()
  ("help", "This algorithm finds the Steiner Tree for a given graph. Nodes with prime indices are terminals.") // help message.
  ("FILE", po::value<string>(&fileName), "Input file path and name.") // takes the graph file name.
  ("firstNTerminals", po::value<int>(&firstNTerminals)->default_value(100),"First N terminals to check.") // the first N terminals to check. If none specified, default is 100 terminals.
  ("showEdges,s", po::value<string>()->implicit_value(""),"Show best Steiner-Graph edges."); // and whether or not to print Steiner tree edges.
  
  po::positional_options_description p;
  p.add("FILE", 1); // fix position as per assignment requirement: graph filename as position 1.
  p.add("firstNTerminals", 2); // and first N terminals to check at position 2.
  
  po::variables_map vm; // intialise variable map to capture command line inputs.
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
  /* end getting command line arguments */
  
  
  /* start get number of nodes */
  ifstream file(fileName);  // read graph file.
  string str; /// read graph file line by line.
  
  getline(file, str);
  int n; /// store n as int for number of edges.
  
  auto it = str.begin();
  parse(it, str.end(), int_[([&n](int i){n = i;})] >> int_);
  n++;
  /* end get number of nodes */
  
  
  /* start read edges and weights from file */
  cout << endl;
  cout << "Getting edges and weights from file..." << endl;
  pair<vector<Edge>,vector<int>> edgesWeights = utils::get_EdgesWeights(n, file);
  file.close();
  /* end read edges and weights from file */
  
  
  /* start generating primes and get number of primes */
  vector<int> primes = utils::gen_Primes(n); // get all primes in number of nodes.
  int primesSize = primes.size(); // get the number of primes.
  
  cout << "There are " << primesSize << " terminals." << endl; // print total number of terminals in graph.
  
  if (firstNTerminals > primesSize) { // if the number of terminals to go through is larger than there are terminals, then use the number of terminals.
    firstNTerminals = primesSize;
  }
  /* end generating primes and get number of primes */
  
  
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
  
  // if openMP is defined,
  #ifdef _OPENMP
  // and if THREAD is defined at compile time,
  int nThreadsUsed = numeric_limits<int>::max(); // variable to check for number of threads used.
  #ifdef THREADS
  // then use number of threads specified,
  #pragma omp parallel for firstprivate(originalAdjList) num_threads(THREADS) 
  #else
  // else let openMP decide.
  #pragma omp parallel for firstprivate(originalAdjList)
  #endif
  #endif
  for (int i=0;i<firstNTerminals;i++) { // for the number of terminals to be looped over,
    vector<vector<Vertex>> adjList =  originalAdjList; // make a copy of the adjacency list.
    const int startTerminal = primes[i]; // get the terminal index that is the current start terminal.
    
    vector<Edge> subgraphEdges; // edge vector to store edges of the Steiner-subgraph.
    vector<bool> inSubgraph(n,false); // boolean vector where node numbers in steiner subgraph are true, else false.
    inSubgraph[startTerminal] = true; // the start terminal should always be in subgraph...
    vector<int> subgraphWeights; // weight vector to store the weights of the edges of the Steiner-subgraph.
    vector<int> dists; // dist and parents to store the distances and parents/predecessors of the calculated shortest-paths from start node.
    vector<int> parents; 
    
    // steiner-tree heuristic looks through each terminal while building the steiner-subgraph. The first terminal is the start terminal.
    int currentPrime = startTerminal; // each terminal to be looped over is the "currentPrime".
    
    for (int j = 0; j<primesSize; j++) { // for each prime/terminal, 
      if (j%500==0){
	if (j+500-1 > primesSize){ // print the group of 500 terminals that are being checked,
	  cout << "Processing terminals number: " << j << " to " << primesSize << endl;
	}
	else{
	  cout << "Processing terminals number: " << j << " to " << j + (500-1) << endl;
	}
      }
      
      myHeap Unvisited(n, currentPrime, inSubgraph); // and initialise the priority queue for the dijkstra algorithm.
      
      pair<vector<int>,vector<int>> shortestDists = dijkstra_alg::alg(n, Unvisited, adjList, currentPrime, inSubgraph); // calculate the shortest distances using dijkstra algorithm.
      dists = shortestDists.first; // get the distances of all the nodes from the current prime, 
      parents = shortestDists.second; // and their parents/predecessors.
      
      int minPrime = utils::get_Min(dists, primes, inSubgraph); // get the minimum of all the prime distances.
      
      int currentWeight = numeric_limits<int>::max(); // initialise current weight of the edge under consideration, for the steiner-heuristic.
      int currentNode = minPrime; // start node for the steiner-heuristic is the current minimum prime-value.
      int pred = parents[currentNode]; // trace the next node on the edge by looking up the parents list.
      
      
      while (!inSubgraph[currentNode]) { // so long as the current node is not in the Steiner-subgraph...
	Edge edge = make_pair(pred,currentNode); // make an edge out of current node and its parent.
	
	currentWeight = dists[currentNode] - dists[pred]; // distance of the current node to its parent (the edge weight) is the shortest distance to the start terminal of the current node, minus that of the parent node.
	// something is wrong if the parent of the current node is a prime, and has non-zero weight. Assert this.
	assert(!((inSubgraph[pred]==false) && (find(primes.begin(), primes.end(), pred)!=primes.end()) && (currentWeight!=0)));
	
	// add the edge and its weight to the steiner-subgraph.
	subgraphEdges.push_back(edge);
	subgraphWeights.push_back(currentWeight);
	inSubgraph[currentNode] = true; // add current node to reflect that it is in the steiner-subgraph.
	
	
	// update to the next edge.
	currentNode = pred;
	pred = parents[currentNode];
	
	// check if the edge has been traced to the start terminal, if yes, then break.
	if (pred == currentNode) {
	  break;
	}
      }
      assert(checker::isTree(subgraphEdges,currentPrime)); // every of these steiner trees found must be a tree...
      currentPrime = minPrime; // in the next iteration, find shortest distances in graph from the terminal that was just added to the Steiner subgraph.
    }
    
    int objVal = 0;
    for_each (subgraphWeights.begin(), subgraphWeights.end(), [&] (int n) { // objective value is the sum of the edge weights.
      objVal += n;
    });
    
    // if openMP is defined.
    #ifdef _OPENMP
    if (omp_get_num_threads() < nThreadsUsed) { // get the lowest number of threads used in the parallel region.
      nThreadsUsed = omp_get_num_threads();
    }
    // use only one thread to update objVals.
    #pragma omp critical
    #endif
    if (objVal<bestObjVal) { // update the best objective value if it is less than before.
      bestObjVal = objVal;
      bestSteinerEdges = subgraphEdges; // update the best Steiner subgraph as well,
      bestStartTerminal = startTerminal; // and the best StartTerminal that allowed this to happen...
    }
  }
  boost::timer::cpu_times times = timer.elapsed(); // once done, stop recording time.
  /* end calculating steiner trees. */
  
  
  /* start checking the best tree. */
  cout << "Checking steiner tree..." << endl;
  
  if (checker::isTree(bestSteinerEdges, bestStartTerminal)) { // check if graph is a tree,
    cout << "Solution is a tree." << endl;
  }
  else {
    cout << "Solution is NOT a tree." << endl;
  }
  checker::isFeasible(bestSteinerEdges,primes); // and contains all terminals.
  #ifdef _OPENMP
  cout << "Number of threads used: " << nThreadsUsed << endl;
  #endif
  cout << endl;
  /* end checking the best tree. */
  
  
  /* start printing values. */
  cout << "TLEN: " << bestObjVal << endl; // print best objective value.
  
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
