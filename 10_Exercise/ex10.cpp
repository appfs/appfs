/*

 * ex10.cpp

 *

 *

 *  Author: Daniel Rehfeldt

 *

 *  Compile: g++ ex10.cpp -o ex10 -O3 -march=native -std=c++14 -lboost_timer  -lboost_system  -pedantic -Wall
 *
 *  Compile with OpenMP:  g++ ex10.cpp -o ex10 -O3 -march=native -std=c++14 -lboost_timer  -lboost_system  -pedantic -Wall -fopenmp
 *
 *
 */

#define NDEBUG
#define SEQHEAP

#include <limits>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cassert>
#include <memory>
#include <string>
#include <array>
#include <algorithm>
#include <boost/spirit/include/qi.hpp>
#include <boost/timer/timer.hpp>
#ifdef SEQHEAP
#include "knheap.C"
#else
#include "binheap.h"
#endif
#ifdef _OPENMP
#include <omp.h>
#endif



using Weight = long long;

struct IntInt
{
      int index;
      int prededge;
};



class Graph{

    public:
        int* deg = nullptr;
        int* term = nullptr;
        int* head = nullptr;
        int* tail = nullptr;
        int* headcprs = nullptr;
        int* tailcprs = nullptr;
        int* startcprs = nullptr;
        int* weightcprs = nullptr;
        int* orgedgecprs = nullptr;
        Weight* edgeweight = nullptr;
        Weight* edgeweightcprs = nullptr;

        int nterms = -1;
        int nnodes = -1;
        int nedges = -1;
        int nmaxedges = -1;

        Graph(int nodes, int maxedges)
        {
            assert(nodes >= 1);
            assert(maxedges >= 0);
            nedges = 0;
            nterms = 0;
            nnodes = nodes;
            nmaxedges = maxedges;

            if( maxedges > 0 )
            {
               // initialize with 0
               deg = new int[nnodes]();
               term = new int[nnodes]();

               head = new int[maxedges];
               tail = new int[maxedges];
               edgeweight = new Weight[maxedges];
            }
        }

        bool isTerm(int i) const;

        void makeTerm(int i);

        void addEdge(int etail, int ehead, Weight eweight);

        // initialize compressed data structures
        void initCompressed();

        // check whether compression was successful
        bool compressedValid() const;

        bool isConnected() const;

        ~Graph()
        {
           delete[] orgedgecprs;
           delete[] weightcprs;
           delete[] edgeweightcprs;
           delete[] headcprs;
           delete[] tailcprs;
           delete[] startcprs;
           delete[] edgeweight;
           delete[] deg;
           delete[] head;
           delete[] tail;
        }
};



bool Graph::isConnected() const
{
     assert(startcprs);
     assert(headcprs);

     bool* visited = new bool[nnodes];
     int* stack = new int[nnodes];

     int size = 0;

     std::fill_n(visited, nnodes, false);

     stack[size++] = 0;
     visited[0] = true;

     int nodecount = 0;

     // DFS
     while( size )
     {
        const int k = stack[--size];
        nodecount++;

        //traverse adjacent edges of k
        const int end = startcprs[k + 1];
        for( int e = startcprs[k]; e != end; e++ )
        {
            const int head = headcprs[e];
            if( !visited[head] )
            {
               visited[head] = true;
               stack[size++] = head;
            }
        }
     }

     delete[] visited;
     delete[] stack;

     if( nodecount != nnodes )
         return false;

     return true;
}

inline bool Graph::isTerm(
     int i
) const
{
   assert(i >= 0 && i < nnodes);
   assert(term);

   return (term[i] != 0);
}

inline void Graph::makeTerm(
    int i
)
{
   assert(i >= 0 && i < nnodes);
   assert(term);

   if( !term[i] )
   {
      nterms++;
      term[i] = 1;
   }
}


inline void Graph::addEdge(
    int etail,
    int ehead,
    Weight eweight
)
{
    assert(ehead >= 0 && ehead < nnodes );
    assert(etail >= 0 && etail < nnodes );
    assert(nedges < nmaxedges);
    assert(eweight >= Weight(0));

    head[nedges] = ehead;
    tail[nedges] = etail;
    edgeweight[nedges++] = eweight;
    deg[ehead]++;
    deg[etail]++;
}


void Graph::initCompressed()
{
    tailcprs = new int[2 * nedges];
    headcprs = new int[2 * nedges];
    orgedgecprs = new int[2 * nedges];
    startcprs = new int[nnodes + 1];
    edgeweightcprs = new Weight[2 * nedges];

    startcprs[0] = deg[0];

    for( int i = 1; i < nnodes; i++ )
        startcprs[i] = startcprs[i - 1] + deg[i];

    assert(startcprs[nnodes - 1] == 2 * nedges);

    startcprs[nnodes] = 2 * nedges;

    auto update_cprs = [this](int node, int adjnode, int edge)
    {
        const int e = --startcprs[node];
        assert(e >= 0);
        assert(node == 0 || e >= startcprs[node - 1]);
        edgeweightcprs[e] = edgeweight[edge];
        tailcprs[e] = node;
        headcprs[e] = adjnode;
        orgedgecprs[e] = edge;
    };

    for( int e = 0; e < nedges; e++ )
    {
        update_cprs(tail[e], head[e], e);
        update_cprs(head[e], tail[e], e);
    }

    assert(startcprs[nnodes - 1] + deg[nnodes - 1] == 2 * nedges);
}



bool Graph::compressedValid() const
{
   if( !startcprs || !edgeweightcprs || !tailcprs || !headcprs || !orgedgecprs )
      return false;

   auto is_adjacent = [this](int node, int nodeadj)
   {
      for( int e = startcprs[node], end = startcprs[node + 1]; e != end; e++ )
         if( tailcprs[e] != node )
            return false;

      for( int e = startcprs[node], end = startcprs[node + 1]; e != end; e++ )
         if( headcprs[e] == nodeadj )
            return true;

      return false;
   };

   for( int e = 0; e < nedges; e++ )
       if( !is_adjacent(tail[e], head[e]) || !is_adjacent(head[e], tail[e]) )
          return false;

   return true;
}

// fills array that marks whether edge i is part of the tree (steineredge[i] = true) or not (steineredge[i] = false)
static std::unique_ptr<bool[]> computeSteinerTree(
    const Graph& graph,
    const int root
)
{
    const int nnodes = graph.nnodes;
    const int nedges = graph.nedges;
    const int nterms = graph.nterms;

    assert(nnodes >= 1);
    assert(nedges >= 1);
    assert(nterms >= 1);
    assert(root >= 0 && root < nnodes);

    auto steineredge = std::make_unique<bool[]>(nedges);

    if( nterms == 1 && graph.isTerm(root) )
       return move(steineredge);

    const int* orgarr = graph.orgedgecprs;
    const int* tailarr = graph.tailcprs;
    const int* headarr = graph.headcprs;
    const int* startarr = graph.startcprs;
    const Weight* weightarr = graph.edgeweightcprs;

    assert(orgarr);
    assert(tailarr);
    assert(startarr);
    assert(headarr);
    assert(weightarr);

    // predecessor and distance array to store incoming edge and distance for each node todo unique pointer
    bool* steinernode = new bool[nnodes];
    int* predarr = new int[nnodes];
    Weight* distarr = new Weight[nnodes];

    // initialize array for Dijkstra calculations
    std::fill_n(distarr, nnodes, std::numeric_limits<Weight>::max());
    std::fill_n(predarr, nnodes, -1);
    std::fill_n(steinernode, nnodes, false);

#ifdef SEQHEAP
    KNHeap<Weight, IntInt> heap(std::numeric_limits<Weight>::max(), std::numeric_limits<Weight>::min());
#else
    BinHeap<Weight, IntInt> heap(2 * nedges);
#endif

    assert(heap.getSize() == 0);

    IntInt dummy;
    dummy.index = root;
    dummy.prededge  = -1;
    heap.insert(Weight(0), dummy);

    distarr[root] = Weight(0);
    steinernode[root] = true;

    int termcount = 0;
    if( graph.isTerm(root) )
       termcount++;

    while( heap.getSize() != 0 )
    {
        Weight distk;

#ifdef SEQHEAP
        heap.deleteMin(&distk, &dummy);
#else
        heap.deleteMin(distk, dummy);
#endif

        assert(distk >= Weight(0));

        const int node = dummy.index;

        assert(node >= 0);

        if( distarr[node] >= distk )
        {
           distarr[node] = distk;
           predarr[node] = dummy.prededge;

           if( graph.isTerm(node) && !steinernode[node] )
           {
              int k = node;

              // add path to current tree
              do
              {
                 int pred;
                 steinernode[k] = true;
                 pred = predarr[k];

                 assert(pred >= 0);
                 assert(headarr[pred] == k);

                 steineredge[orgarr[pred]] = true;
                 dummy.index = k;
                 dummy.prededge = pred;

                 distarr[k] = Weight(0);

                 heap.insert(Weight(0), dummy);

                 k = tailarr[pred];

              }
              while( !steinernode[k] );

              // all terminals connected?
              if( ++termcount == nterms )
                 break;
           }

           // traverse adjacent edges of node
           const int end = startarr[node + 1];
           for( int e = startarr[node]; e != end; e++ )
           {
               const int head = headarr[e];

               if( distarr[head] > distk + weightarr[e] )
               {
                  // update
                  distarr[head] = distk + weightarr[e];
                  dummy.index = head;
                  dummy.prededge = e;
                  heap.insert(distarr[head], dummy);
                  //heap.insert(distarr[head], e);
               }
           }
        }
    }

    delete[] steinernode;
    delete[] distarr;
    delete[] predarr;

    return move(steineredge);
}

static bool isTreeValid(
      const Graph& graph,
      bool* steinertree,
      const int root
)
{
   assert(steinertree);

   const int nnodes = graph.nnodes;
   const int nedges = graph.nedges;
   const int nterms = graph.nterms;
   const int* orgarr = graph.orgedgecprs;
   const int* headarr = graph.headcprs;
   const int* startarr = graph.startcprs;

   assert(orgarr);
   assert(startarr);
   assert(headarr);

   bool* visited = new bool[nnodes];
   int* stack = new int[nnodes];

   int size = 0;
   int nsolnodes = 0;
   int termcount = 0;

   std::fill_n(visited, nnodes, false);

   stack[size++] = root;
   visited[root] = true;

   // DFS
   while( size )
   {
      const int k = stack[--size];

      nsolnodes++;

      if( graph.isTerm(k) )
         termcount++;

      //traverse adjacent edges of k
      const int end = startarr[k + 1];
      for( int e = startarr[k]; e != end; e++ )
      {
          const int head = headarr[e];
          if( !visited[head] && steinertree[orgarr[e]] )
          {
             visited[head] = true;
             stack[size++] = head;
          }
      }
   }

   delete[] visited;
   delete[] stack;

   int nsoledges = 0;

   for( int e = 0; e < nedges; e++ )
      if( steinertree[e] )
         nsoledges++;

   if( nsoledges != nsolnodes - 1 )
   {
       std::cout << "not a tree "  << std::endl;
       return false;
   }

   if( termcount != nterms )
   {
       std::cout << "not all terminals reached " << termcount << " < " << nterms <<  std::endl;
       return false;
   }

   return true;
}

// adds prime vertices as terminals
static void addTermsPrime(
      Graph& graph
)
{
   const int nnodes = graph.nnodes;

   // Sieve's prime algorithm

   bool* primecands = new bool[nnodes + 1];

   std::fill_n(primecands, nnodes + 1, true);

   const int sqrn = sqrt(nnodes);

   for( int k = 4; k <= nnodes; k += 2 )
      primecands[k] = false;

   for( int i = 3; i <= sqrn; i += 2 )
      if( primecands[i] )
      {
         const int currprime = i;

         for( int k = currprime * currprime; k <= nnodes; k += 2 * currprime )
            primecands[k] = false;
      }

   if( nnodes >= 2 )
      graph.makeTerm(1);

   for( int i = 3; i <= nnodes; i += 2 )
      if( primecands[i] )
         graph.makeTerm(i - 1);

   delete[] primecands;

}


static Graph loadGraph(
   const char* filename
)
{
    std::ifstream file (filename, std::ifstream::in);

    if( file.fail() )
    {
       std::cerr << "file could not be opened" << std::endl;
       exit(EXIT_FAILURE);
    }

    int nnodes;
    int nedges;

    if( !(file >> nnodes >> nedges) || nedges <= 0 || nnodes <= 1 )
    {
        std::cerr << "file syntax error" << std::endl;
        exit(EXIT_FAILURE);
    }

    Graph graph(nnodes, nedges);

    std::string strline;

    using namespace boost::spirit;
    using qi::int_;
    using qi::phrase_parse;
    using ascii::space;

    int etail, ehead;
    Weight eweight;

    while( std::getline(file, strline) )
    {
        auto it = strline.begin();

        bool success = phrase_parse(it, strline.end(),
                 int_[([&etail](int i){ etail = i; })] >> int_[([&ehead](int i){ ehead = i; })] >> int_[([&eweight](Weight i){ eweight = Weight(i); })]
                          , space);

        if( success && it == strline.end() )
           graph.addEdge(etail - 1, ehead - 1, eweight);
    }

    file.close();

    std::cout << "loaded graph;" << " nodes: " << nnodes << ", edges: " << nedges  << " ... ";

    return graph;
}


int main(
    int argc,
    char* argv[]
)
{
    // check number of parameters
    if( argc != 3 )
    {
       // tell the user how to run the program
       std::cerr << "Usage: " << argv[0] << " filename " << "number_of_starting_nodes"  << std::endl;
       exit(EXIT_FAILURE);
    }

    const int nstarts = std::stoi(argv[2]);

    if( nstarts <= 0 )
    {
       std::cout << "number of starting nodes should be >= 1" << std::endl;
       exit(EXIT_FAILURE);
    }

    Graph graph = loadGraph(argv[1]);
    addTermsPrime(graph);

    std::cout << "terminals: " << graph.nterms << std::endl;

    graph.initCompressed();
    assert(graph.compressedValid());

    if( !graph.isConnected() )
    {
       std::cout << "graph unconnected" << std::endl;
       exit(EXIT_FAILURE);
    }

    const int nedges = graph.nedges;
    const int nruns = std::min(nstarts, graph.nterms);

    bool* incumbent(new bool[nedges]);
    int* startterms(new int[graph.nnodes]);

    for( int i = 0, t = 0; t < nruns; i++ )
       if( graph.isTerm(i) )
          startterms[t++] = i;

    int bestroot = -1;
    Weight bestobj = std::numeric_limits<Weight>::max();


    using namespace boost::timer;
    cpu_timer timer;

    // run heuristic from first nruns many terminals (prime nodes)
    #pragma omp parallel for
    for( int r = 0; r < nruns; r++ )
    {
       const int root = startterms[r];

       assert(graph.isTerm(root));

       std::unique_ptr<bool[]> steinertree = computeSteinerTree(graph, root);

       assert(isTreeValid(graph, steinertree, root));

       Weight obj = Weight(0);
       for( int e = 0; e < nedges; e++ )
          if( steinertree[e] )
             obj += graph.edgeweight[e];

#ifdef _OPENMP
#pragma omp critical
       {
          int t = omp_get_thread_num();
          int tcount = omp_get_num_threads();
          std::cout << "thread " << t  << " of " << tcount << " finished" << std::endl;
       }
#endif

       #pragma omp critical
       if( obj < bestobj )
       {
          bestobj = obj;
          bestroot = root;
          for( int i = 0; i < nedges; i++ )
             incumbent[i] = steinertree[i];
       }
    }

    cpu_times times = timer.elapsed();

    delete[] startterms;

    assert(bestroot >= 0);

    if( !isTreeValid(graph, incumbent, bestroot) )
       std::cout << "FAILED WITH FINAL TREE" << std::endl;
    else
       std::cout << "FINAL TREE OK!" << std::endl;

    delete[] incumbent;
    assert(bestobj < std::numeric_limits<Weight>::max());

    std::cout << "TLEN: " << bestobj << std::endl;
    std::cout << "TIME: "  << times.user / 1e9 << " s" << std::endl;
    std::cout << "WALL: "  << times.wall / 1e9 << " s" << std::endl;


    return EXIT_SUCCESS;
}
