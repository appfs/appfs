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

#include "utils.h"
#include "checker.h"
#include "heap.h"
#include "steiner.h"

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
  ("showEdges,s", po::value<string>()->implicit_value(""),"Show best Steiner-Graph edges."); // -s flag: whether or not to print Steiner tree edges.
  
  po::positional_options_description p;
  p.add("FILE", 1); // fix position as per assignment requirement: graph filename as position 1.
  p.add("firstNTerminals", 2); // and first N terminals to check at position 2.
  
  // intialise variable map to capture command line inputs.
  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).
  options(desc).positional(p).run(), vm);
  po::notify(vm);
  
  // display help message and end program.
  if (vm.count("help")) {
    cout << desc << endl;
    return 0;
  }
  
  // if no filename, return error.
  if (!vm.count("FILE")) {
    cout << "No graph file." << endl;
    return -1;
  }
  /* end getting command line arguments */
  
  
  /* start get number of nodes */
  ifstream file(fileName);
  string str;
  
  getline(file, str);
  int n; // n for number of edges in graph.
  
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
  
  // if the number of terminals to go through is larger than there are terminals, then use the number of terminals.
  if (firstNTerminals > primesSize) {
    firstNTerminals = primesSize;
  }
  /* end generating primes and get number of primes */
  
  
  /* start building graph and initalising needed stuff for Steiner subgraph. */
  boost::timer::cpu_timer timer;
  vector<Edge> bestSteinerEdges; // to store the best steiner subgraph edges.
  long bestObjVal = numeric_limits<long>::max(); // to store the best objective value.
  int bestStartTerminal = numeric_limits<int>::max(); // to store the best start terminal that yielded the above objective value.
  
  cout << "Building graph..." << endl;
  // build the original adjacency list. Each thread will get a copy of this adjList.
  vector<vector<Vertex>> originalAdjList = utils::build_adjList(n, edgesWeights.first, edgesWeights.second);
  /* end building graph and initalising needed stuff for Steiner subgraph. */
  
  
  /* start calculating steiner trees. */
  cout << "Calculating steiner trees..." << endl;
  
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
  // for the number of terminals to be looped over, make a copy of the adjacency list.
  for (int i=0;i<firstNTerminals;i++) {
    vector<vector<Vertex>> adjList =  originalAdjList;
    const int startTerminal = primes[i]; // get the node index that is the current start terminal.
    vector<bool> isPrimes = utils::isPrime(n, primes); // generate a boolean array to check against which index is a prime.

    // initialise the priority queue for the steiner heuristic.
    myHeap Unvisited(n, startTerminal);
    
    // calculate the steiner tree.
    pair<vector<Edge>,vector<int>> subgraphEdgesWeights = steiner::alg(n, Unvisited, adjList, startTerminal, isPrimes); 
    
    // get the steiner subgraph edges and their respective weights.
    vector<Edge> subgraphEdges = subgraphEdgesWeights.first;
    vector<int> subgraphWeights = subgraphEdgesWeights.second;
    
    assert(checker::isTree(subgraphEdges,startTerminal)); // every of these steiner trees found must be a tree...
    
    long objVal = 0;
    // objective value is the sum of the edge weights.
    for_each (subgraphWeights.begin(), subgraphWeights.end(), [&] (int n) { 
      objVal += n;
      assert(objVal >= 0); // objective value musn't overflow.
      }
    );
    
    // if openMP is defined.
    #ifdef _OPENMP
    // get the lowest number of threads used in the parallel region.
    if (omp_get_num_threads() < nThreadsUsed) {
      nThreadsUsed = omp_get_num_threads();
    }
    // use only one thread to update objVals.
    #pragma omp critical
    #endif
    // update the best objective value, steiner subgraph and start terminal if it is less than before.
    if (objVal < bestObjVal) {
      bestObjVal = objVal;
      bestSteinerEdges = subgraphEdges;
      bestStartTerminal = startTerminal;
    }
  }
  boost::timer::cpu_times times = timer.elapsed();
  /* end calculating steiner trees. */
  
  
  /* start checking the best tree. */
  cout << "Checking steiner tree..." << endl;
  
  if (checker::isTree(bestSteinerEdges, bestStartTerminal)) {
    cout << "Solution is a tree." << endl;
  }
  else {
    cout << "Solution is NOT a tree." << endl;
  }
  
  checker::isFeasible(bestSteinerEdges,primes);
  #ifdef _OPENMP
  cout << "Number of threads used: " << nThreadsUsed << endl;
  #endif
  cout << endl;
  /* end checking the best tree. */
  
  
  /* start printing values. */
  cout << "TLEN: " << bestObjVal << endl;
  
  // print the edges of the subgraph if -s is present.
  if (vm.count("showEdges")) {
    cout << "TREE: ";
    for (auto edge=bestSteinerEdges.begin(),end=bestSteinerEdges.end(); edge!=end; edge++) {
      cout << "("<<edge->first<<","<<edge->second<<") ";
    }
    cout << endl;
  }
  // print CPU- and Wall-Time. 
  cout << "TIME: " << setprecision(4) << times.user / 1e9 << "s" << endl;
  cout << "WALL: " << setprecision(4) << times.wall / 1e9 << "s" << endl;
  cout << endl;
  /* end printing values. */
  
  return 0;
}
