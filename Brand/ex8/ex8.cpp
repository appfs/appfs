/*!  @author Michael Brand
*    @Excercise 8
*    @date 17.07.2017
*
*   Algorithm to find Steiner tree on a graph where primes count as termainals.
*
*   I decided to leave all code in one file since its mainly consistent of two bigger algorithms
*   1. Dijkstra
*   2. Analyzing Dijkstra output in main-function
*   I don't think it gets to complicated reading it from top to bottom.
*/

#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <stdio.h>
#include <cstdio>
#include <ctime>
#include <chrono>
#include <vector>
#include <climits>
#include <utility>                          // for std::pair

#include <boost/config.hpp>
#include <boost/utility.hpp>                // for boost::tie
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/property_map/property_map.hpp>

using namespace std;


typedef int vertex_;  /*!< defines a vertex as an int */
typedef int weight_;  /*!< defines a weight as an int */

/**Edge
 * pair of ints containing the weight and
 * the vertex pointed to. This is useful
 * for the adjacency list
 */
typedef pair<vertex_, weight_> Edge;

/**Graph
 * An adjacency list representing the graph.
 * graph[i] returns a list with Edge elements.
 *
 */
typedef vector< vector<Edge> > Graph;


/**struct pq_compare
 * compare structure for edges
 * allows me to compare two edges in a graph
 * this way I can sort edges for any vertex under consideration of their weight
 */   
struct pq_compare {
    bool operator() (const Edge i, const Edge j) const{
    return (i.second <= j.second); }
};

/*! \fn bool isPrime(int number)
    \brief checks if number is a prime.
    \param number number to be checked.
*/
bool isPrime(int number){

    if(number < 2) return false;
    if(number == 2) return true;
    if(number % 2 == 0) return false;
    for(int i=3; (i*i)<=number; i+=2){
        if(number % i == 0 ) return false;
    }
    return true;

}

/*! \fn std::vector<int> getRequiredPrimes(int numV)
    \brief lists all primes <= numV in a vector.
    \param numV number to be checked.
    \return vector of primes
*/
std::vector<int> getRequiredPrimes(int numV){
  std::vector<int> primes;
  for(int i=3; i<=numV; i++){
    if(isPrime(i)) primes.push_back(i);
  }
  return primes;
}


/*! \fn std::vector<vertex_> dijkstra(const Graph &graph, vertex_ root, vector<int> &remPrimes)
*   \brief modified dijkstra algorithm used for steiner tree problem
*   \param &graph the graph we are working on.
*   \param root source vertex from which we want to calculate the distance
*   \&remPrimes vector of remaining prime no.
*   \return vector path that consists of
*     path[0] distance to terminal
*     path[1] terminal index
*     path[2] pre of path[1]
*     ...
*     path[n] pre of path[n-1]
*     path[n] is vertex index which has source index as pre
*/
std::vector<vertex_> dijkstra(const Graph &graph, vertex_ root, vector<int> &remPrimes, bool stNodeStructLoc[]) {

  std::vector<vertex_> path;
  std::vector<weight_> dist(graph.size(), INT_MAX);
  /* A set helps insertion and insert/erase/find operations in logarithmic time.
   * This set maintains Edge(distance,vertex number) sorted on basis of distance
   */
  set< Edge , pq_compare> pq;
  set< Edge , pq_compare > ::iterator it;


  vector<vertex_> pre(graph.size(), (-1)); /*!< vevtor of predecessors */
  int u,v,wt;
  int nPrime = 0; /*!< next prime - closest prime that is remaining in &remPrimes */

  dist[root] = 0;
  pq.insert(Edge(root,0));

  while(pq.size() != 0){
    bool found = false;
    it = pq.begin();
    u = it->first;
    pq.erase(it);
    if(isPrime(u+1)){
        for(int chk=0; chk<remPrimes.size(); chk++){
          if((remPrimes[chk])==(u+1)){
            nPrime=u;
            found = true;
            break;
          }
        }
    }
    if(found) break;
    
    for(vector<Edge>::const_iterator ni = graph[u].begin(); ni != graph[u].end(); ni++){
      v  = ni->first;
      wt = ni->second;
      if(stNodeStructLoc[v]){
        continue;
      } 
      if(dist[v] > dist[u] + wt){
        pre[v] = u;
        if(dist[v] != INT_MAX){
          pq.erase(Edge(v,dist[v]));
        }
        dist[v] = dist[u] + wt;
        pq.insert(Edge(v,dist[v]));
      }
    
    }
  }
  /**
  * If source was internal node of steiner subgraph no new terminal is found
  * so we return vector of INT_MAX element
  */
  if(nPrime==0){
    path.push_back(INT_MAX);
    return path;
  }
  int distTerm = dist[nPrime];  /*!< distance to chosen terminal */
  /*! Create path
  *   save all nodes on path to a vector
  *   add distance of edges at the end at the top
  *   do not at the chosen root. does not need to be added to the subgraph anymore
  */
  path.push_back(nPrime);
  int lN = nPrime;
  while(lN != root){
    lN = pre[lN];
    path.push_back(lN);
  }
  path.insert(path.begin(),distTerm);
  return path;
}

int main (int argc, char* argv[]) {

  /**
  * start timers for cpu and wall time
  */
  clock_t cpu0 = clock();
  auto   wall0 = chrono::system_clock::now();
  if( argc != 2){
      fprintf(stderr, "Call the program as: %s 'filename.gph'", argv[0]);
      exit(EXIT_FAILURE);
  }
  ifstream    file(argv[1]);
  string      line;
  if(!file){
    fprintf(stderr, "Could not open file.");
    return -1;
  }
  /**
  * read number of vertices and edges
  */
  getline(file, line, ' ');
  const int numV = stoi(line);
  getline(file, line, '\n');
  const int numE = stoi(line);
  /**
  * create graph and steiner subgraph structures
  */
  Graph graph(numV);
  Graph steinerGraph(numV);
  bool stNodeStruct[numV] = { 0 };;
  int noStEdges = 0;  /*!< #edges in steiner tree */
  int stEdWght = 0;   /*!< obj value of steiner tree */
  int stNodes = 1;
  /**
  * read ín given gph file
  */
  while( getline(file, line) ){
    stringstream linestream(line);
    string       vertex1, vertex2, weight;
    try{
      getline(linestream, vertex1, ' ');
      getline(linestream, vertex2, ' ');
      getline(linestream, weight, '\n');
      /**
      * add both directions of edge to the graph since undirected
      * index switch applies: 1 --> 0, 1 --> 2, etc.
      */
      graph[stoi(vertex1)-1].push_back(Edge(stoi(vertex2)-1, stoi(weight)));
      //std::sort (graph[stoi(vertex1)-1].begin(), graph[stoi(vertex1)-1].end(), sortEdges);
      graph[stoi(vertex2)-1].push_back(Edge(stoi(vertex1)-1, stoi(weight)));
      // std::sort (graph[stoi(vertex2)-1].begin(), graph[stoi(vertex2)-1].end(), sortEdges);
    }catch (invalid_argument& ia){
      //when data is not a digit,
      //std::stoi throws an invalid argument exception
    } catch ( ... ){}
  }//while
  file.close();
  /**
  * get vector of primes that need to be connected in steiner tree
  */
  std::vector<int> remainingPrimes = getRequiredPrimes(numV);
  bool initState=true;
  /**
  * we initialize the steiner subgraph with terminal 2 (initState=true)
  * in the following (initState=false) we start dijkstra from every single node in the
  * subgraph and collect in every iteration the closest new terminal to the existing subgraph
  * -> saved in lovalSelection
  * we copy the result into an existing vector -> globalSelection
  * after all localSelections have been calculated we iterate through globalSelection
  * and coose the terminal that has the minimal distance (saved in globalSelection[i][0])
  */
  do{
    std::vector< std::vector<vertex_>> globalSelection;
    int distance = INT_MAX;
    int choice = -1;
    if(initState){
      std::vector<vertex_> localSelection = dijkstra(graph, 1, remainingPrimes, stNodeStruct);
      globalSelection.push_back(localSelection);
      for(int j=0; j<globalSelection.size(); j++){
        if(globalSelection[j][0]<distance){
         distance = globalSelection[j][0];
         choice = j;
        }
      }
      initState=false;
    }
    else{ 
      for(int i=0; i<steinerGraph.size(); i++){
        if(!(steinerGraph[i].empty())){
          std::vector<vertex_> localSelection = dijkstra(graph, i, remainingPrimes, stNodeStruct);
          globalSelection.push_back(localSelection);
        }
      }
      for(int j=0; j<globalSelection.size(); j++){
        if(globalSelection[j][0]<distance){
          distance = globalSelection[j][0];
          choice = j;
        }
      }
    }
    /**
    * add edges by pre-information:
    * globalSelection consists of path-vectors
    * which have certain structure
    * --> see dijkstra algo info
    */
    for(int pathNode = 1; pathNode < globalSelection[choice].size()-1; pathNode++){
      int from = globalSelection[choice][pathNode+1];
      int to = globalSelection[choice][pathNode];
      stNodes++;
      /*!
      * iterate over from-node-edges
      * search for relevant edge and add it to steinerGraph including its weight
      */
      for(int eIndx = 0; eIndx < graph[from].size(); eIndx++){
        if(graph[from][eIndx].first==to){
          steinerGraph[from].push_back(Edge(to, graph[from][eIndx].second));
          steinerGraph[to].push_back(Edge(from, graph[from][eIndx].second));
          noStEdges++;
          stEdWght += graph[from][eIndx].second;
          stNodeStruct[from] = true;
          stNodeStruct[to] = true;
        }
      }
    }
    /**
    * delete prime from remainingPrimes
    */
    for(int chk=0; chk<remainingPrimes.size(); chk++){
      if((remainingPrimes[chk]==(globalSelection[choice][1]+1))){
        remainingPrimes.erase(remainingPrimes.begin()+chk);
        break;
      }

    }
    globalSelection.clear();
  }while(remainingPrimes.size()>0);
  /**
  * collection stats
  */
  //Stats
  fprintf(stdout, "\n");
  fprintf(stdout, "Original graph:\n#Vert: \t %i\n#Edges \t %i\n", numV, numE);
  fprintf(stdout, "\n");
  fprintf(stdout, "Steiner graph:\n#Vert: \t %i\n#Edges \t %i\nObjVal \t %i\n", stNodes, noStEdges, stEdWght);
  
  fprintf(stdout, "\n");
  double cpuTime = (clock() - cpu0) / (double) CLOCKS_PER_SEC;
  chrono::duration<double> wallDur = (chrono::system_clock::now() - wall0);
  double wallTime = wallDur.count();
  fprintf(stdout, "Finished in %f seconds [CPU Clock] and %f seconds [Wall Clock] \n", cpuTime, wallTime);
  fprintf(stdout, "\n");
  return 0;

}

