/*
 * dijkstra.h
 *
 *  Created on: Jun 19, 2017
 *      Author: july
 */

#ifndef DIJKSTRA_H_
#define DIJKSTRA_H_

#include <vector>

class dijkstra {
public:
	dijkstra(std::vector<int>, std::vector<std::pair<int, int> >);

	std::vector<int> computeShortestPath(unsigned int);

private:
	std::vector<int> weights;
	std::vector<std::pair<int, int> > edges;
};

#endif /* DIJKSTRA_H_ */
