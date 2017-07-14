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

//output of vertices and weights
#define OUTPUT 0

using namespace std;


typedef int vertex_;
typedef int weight_;

/* Edge
 * pair of ints containing the weight and
 * the vertex pointed to. This is useful
 * for the adjacency list
 */
typedef pair<vertex_, weight_> Edge;

/* Graph
 * An adjacency list representing the graph.
 * graph[i] returns a list with Edge elements.
 *
 */
typedef vector< vector<Edge> > Graph;

struct pq_compare {
    bool operator() (const Edge i, const Edge j) const{
    return (i.second <= j.second); }
};


bool isPrime(int number){

    if(number < 2) return false;
    if(number == 2) return true;
    if(number % 2 == 0) return false;
    for(int i=3; (i*i)<=number; i+=2){
        if(number % i == 0 ) return false;
    }
    return true;

}
std::vector<int> getRequiredPrimes(int numV){
  std::vector<int> primes;
  for(int i=3; i<=numV; i++){
    if(isPrime(i)) primes.push_back(i);
  }
  return primes;
}

std::vector<vertex_> dijkstra(const Graph &graph, vertex_ root, vector<int> &remPrimes) {

  //fprintf(stdout, "-----------------------------------");
  std::vector<vertex_> path;
  std::vector<weight_> dist(graph.size(), INT_MAX);
  /* A set helps insertion and insert/erase/find operations in logarithmic time.
   * This set maintains Edge(distance,vertex number) sorted on basis of distance
   */
  set< Edge , pq_compare> pq;
  set< Edge , pq_compare > ::iterator it;

  vector<vertex_> pre(graph.size(), (-1));
  int u,v,wt;
  int nPrime;

  dist[root] = 0;
  pq.insert(Edge(root,0));

  while(pq.size() != 0){
    bool found = false;
    it = pq.begin();
    u = it->first;
    int tmpW = it->second;
    //fprintf(stdout, "looking at vertex: (%i, wght: %i)\n", u+1,tmpW);
    pq.erase(it);
    if(isPrime(u+1)){
        //cout << "isPrime() -> true for: " << u+1 << endl;
        for(int chk=0; chk<remPrimes.size(); chk++){
          if((remPrimes[chk])==(u+1)){
            //cout << "prime is new! : " << u+1 << endl;
            //remPrimes.erase(remPrimes.begin()+chk);
            nPrime=u;
            pq.clear();
            found = true;
            break;
          }
        }
    }
    if(found) break;
    for(vector<Edge>::const_iterator ni = graph[u].begin(); ni != graph[u].end(); ni++){
      v  = ni->first;
      wt = ni->second;
      //fprintf(stdout, "considering edge: (%i,%i,%i)\n", u+1,v+1,wt);
      if(dist[v] > dist[u] + wt){
        //fprintf(stdout, "new pre\n");
        pre[v] = u;
        if(dist[v] != INT_MAX){
          //fprintf(stdout, "erasing edge (%i,%i)", v+1, dist[v]);
          pq.erase(Edge(v,dist[v]));

        }
        dist[v] = dist[u] + wt;
        //fprintf(stdout, "addingEdge: (%i,%i)\n", v+1, dist[v]);
        pq.insert(Edge(v,dist[v]));
      }
    
    }
  }//while

  //fprintf(stdout, "dijkstra: calc complete\n");


  //fprintf(stdout, "chosen prime: %i\n", nPrime+1);

  int distTerm = dist[nPrime];

  path.push_back(nPrime);
  int lN = nPrime;
  while(lN != root){
    path.push_back(pre[lN]);
    lN = pre[lN];
  }
  //path.push_back(root);

  path.insert(path.begin(),distTerm);
  
  //fprintf(stdout, "path: root: %i terminal: %i\n", root+1, nPrime+1);
  //fprintf(stdout, "distance: %i\n", path[0]);
  //for(int i=1; i<path.size(); i++){
    //fprintf(stdout, "%i, pre: %i\n", path[i]+1, pre[path[i]]+1);
  //}
  
  //fprintf(stdout, "exit dijkstra\n");
  return path;
} //dijkstra



int main (int argc, char* argv[]) {

    //start timers for cpu and wall time
    clock_t cpu0 = clock();
    auto   wall0 = chrono::system_clock::now();

    //check for right number of arguments
    if( argc != 2){
        cout << "Call the program as: "
             << argv[0] << " filename.gph" << endl;
        exit(EXIT_FAILURE);
    }


    ifstream    file(argv[1]);
    string      line;

    if(!file){
        cout << "Could not open file" << endl;
        return 1;
    }

    //read number of vertices and edges
    getline(file, line, ' ');
    const int numV = stoi(line);
    getline(file, line, '\n');
    const int numE = stoi(line);
    
    Graph graph(numV);
    Graph steinerGraph(numV);
    int noStEdges = 0;
    int stEdWght = 0;
    
    //loop over all lines in the file
    while( getline(file, line) ){

      stringstream linestream(line);
      string       vertex1, vertex2, weight;

      try{
        getline(linestream, vertex1, ' ');
        getline(linestream, vertex2, ' ');
        getline(linestream, weight, '\n');

        //add both directions, since undirected graph
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

    //fprintf(stdout, "NumV: %i\n", numV);
    std::vector<int> remainingPrimes = getRequiredPrimes(numV);

    //fprintf(stdout, "graph size: %i\n",  graph.size());
    /*OUTPUT

    for(int i=0; i<graph.size(); i++){
      fprintf(stdout, "from: %i | \n", i);
      for(int j=0; j<graph[i].size(); j++){
        fprintf(stdout, "\t\tto: %i | weight: %i\n", graph[i][j].first, graph[i][j].second);
      }
    }
    */
    bool initState=true;

    do{

      //fprintf(stdout, "Begin:> Remaining Primes count: %i\n", remainingPrimes.size());

      std::vector< std::vector<vertex_>> globalSelection;
      int distance = INT_MAX;
      int choice = -1;
      
      if(initState){
          
        //fprintf(stdout, "init calling dijkstra w/ source 2:\n");
        std::vector<vertex_> localSelection = dijkstra(graph, 1, remainingPrimes);

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

            //fprintf(stdout, "calling dijkstra w/ source %i:\n", i+1);
            std::vector<vertex_> localSelection = dijkstra(graph, i, remainingPrimes);

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


      //add Edges by Pre-Information
      for(int pathNode = 1; pathNode < globalSelection[choice].size()-1; pathNode++){


        int to = globalSelection[choice][pathNode];
        int from = globalSelection[choice][pathNode+1];



        //iterate over from-node-edges
        //search for relevant edge and add it to steinerGraph including its weight
        for(int eIndx = 0; eIndx < graph[from].size(); eIndx++){
          if(graph[from][eIndx].first==to){
            steinerGraph[from].push_back(Edge(to, graph[from][eIndx].second));
            steinerGraph[to].push_back(Edge(from, graph[from][eIndx].second));
            noStEdges++;
            stEdWght += graph[from][eIndx].second;
            //fprintf(stdout, "Adding steinerGraph-edge, from: %i, to: %i, weight: %i\n", from+1, to+1, graph[from][eIndx].second);
            //fprintf(stdout, "Adding steinerGraph-edge, from: %i, to: %i, weight: %i\n", to+1, from+1, graph[from][eIndx].second);
          }
        }
      }

      //delete prime from remainingPrimes
      //fprintf(stdout, "search for terminal: %i\n", globalSelection[choice][1]+1);
      for(int chk=0; chk<remainingPrimes.size(); chk++){
        //fprintf(stdout, "check pos %i --> %i\n", chk, remainingPrimes[chk]);

        if((remainingPrimes[chk]==(globalSelection[choice][1]+1))){
          //fprintf(stdout, "deleting terminal %i at position: %i\n", (remainingPrimes[chk]), chk);
          remainingPrimes.erase(remainingPrimes.begin()+chk);
          break;
        }
      }

      //fprintf(stdout,"Remaining Primes count: %i\n", remainingPrimes.size());

      globalSelection.clear();

    }while(remainingPrimes.size()>0);


    //Stats
    fprintf(stdout, "Original graph:\n#Vert: \t %i\n#Edges \t %i\n", numV, numE);
    //Output of SteinerGraph

    int stNodes = 0;

    
    //fprintf(stdout, "Steiner Nodes: \n");
    for(int i=0; i<steinerGraph.size(); i++){
      if(!(steinerGraph[i].empty())){
        stNodes++;
      //  fprintf(stdout, "%i,", i+1);
      } 

    }
    fprintf(stdout, "\n");
  

    fprintf(stdout, "Steiner graph:\n#Vert: \t %i\n#Edges \t %i\nObjVal \t %i\n", stNodes, noStEdges, stEdWght);

    
  
  //fprintf(stdout, "RESULT VERTEX %i \nRESULT DIST %i \n", vertex, maxdist);

  double cpuTime = (clock() - cpu0) / (double) CLOCKS_PER_SEC;
  chrono::duration<double> wallDur = (chrono::system_clock::now() - wall0);
  double wallTime = wallDur.count();

  fprintf(stdout, "Finished in %f seconds [CPU Clock] and %f seconds [Wall Clock] \n", cpuTime, wallTime);


  return 0;

}

