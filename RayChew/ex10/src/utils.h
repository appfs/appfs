/* -- Includes -- */
/* C++ includes. */
#include <iostream> /* for std::cout, std::ofstream */
#include <fstream> /* for fstream::app */
#include <utility> /* for std::pair */
#include <vector> /* for std::vector */
#include <iterator> /* for std::distance */
#include <set> /* for std::set */
#include <map> /* for std::map */
#include <iomanip> /* for std::setprecision */
#include <assert.h> /* for asserts */ 

#ifdef _OPENMP
#include <omp.h> /* for openMP pragmas */
#endif

/* Boost qi parser, program options and timer includes. */
#include <boost/spirit/include/qi.hpp>
#include <boost/timer/timer.hpp>
#include <boost/program_options.hpp>

namespace po = boost::program_options;
using namespace boost::spirit;
using qi::int_;
using qi::double_;
using qi::parse;

#ifndef UTILS_H
#define UTILS_H
#define NDEBUG

using Edge = std::pair<int, int>; // vector to store std::pair of two vertex indices.
using Vertex = std::pair<int,int>; // vector to store std::pair of vertex index and weight.

namespace utils {
  std::vector<int> get_Nodes(std::vector<Edge>& edges);
  
  std::pair<std::vector<Edge>,std::vector<int>> get_EdgesWeights (int& n, std::ifstream& file);
  
  std::vector<int> gen_Primes(int& n);

  int get_Min(std::vector<int>& dists, std::vector<int>& primes, std::vector<bool>& inSubgraph);
  
  std::vector<std::vector<Vertex>> build_adjList(int& n, std::vector<Edge>& edges, std::vector<int>& weights);

  std::vector<std::vector<int>> build_adjList(int& n,std::vector<Edge>& edges);
}

#endif