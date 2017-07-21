#include "utils.h"

/* Boost Bimap Container includes for own algorithm. */
#include <boost/bimap.hpp>
#include <boost/bimap/multiset_of.hpp>
#include <boost/bimap/support/lambda.hpp> /* for bimap::_data and bimap::_key */
//#include <boost/bind.hpp> /* for boost::bind in sorting of vector<pair<int,int>> */

#ifndef M2ALG_H
#define M2ALG_H

namespace bm = boost::bimaps;
using bimap = bm::bimap<int, boost::bimaps::multiset_of<int,std::less<int>>>;

namespace m2alg {
  bimap build_unvisited(int& n, int& startTerminal, std::vector<std::pair<int,int>>& edges);

  std::pair<std::vector<int>,std::vector<int>> m2Algorithm (int& n, bimap& Unvisited, std::vector<std::vector<std::pair<int,int>>>& adjList, int& startTerminal);

  int get_NeighbourIndex(int& currentNode, int& neighbour, std::vector<std::vector<vertex>>& adjList);
};

#endif
