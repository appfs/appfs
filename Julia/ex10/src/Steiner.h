/*
 * Steiner.h
 *
 * 	\brief     My steiner problem solver for ex8
 *  \details   This class solves a steiner tree problem
 *  \author    Julia Baumbach
 *  \date      02.07.2017
 */

#ifndef STEINER_H_
#define STEINER_H_

#include "Dijkstra.h"

/*
 * \typedef Edges which representes all edges in a graph in a list
 */
using Edges = std::vector<std::pair<int, int> >;

class Steiner {
public:
	Steiner(std::vector<int>);
	Edges solveSteiner(SortedEdges&, unsigned int, unsigned int);
	int getObjectiveValue();

private:
	std::vector<int> terminals;
	int objectiveValue;

	int findNearestTerminal(WeightMap);
};

#endif /* STEINER_H_ */
