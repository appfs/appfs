/*
 * SteinerSolver.h
 *
 * 	\brief     My steiner problem solver for ex8
 *  \details   This class solves a steiner tree problem
 *  \author    Julia Baumbach
 *  \date      02.07.2017
 */

#ifndef STEINERSOLVER_H_
#define STEINERSOLVER_H_

#include "DijkstraSolver.h"

/*
 * \typedef Edges which representes all edges in a graph in a list
 */
using Edges = std::vector<std::pair<int, int> >;

class SteinerSolver {
public:
	SteinerSolver(std::vector<int>);
	~ SteinerSolver();
	Edges solveSteiner(SortedEdges&, unsigned int, unsigned int);
	int getObjectiveValue();

private:
	std::vector<int>* terminals;
	int objectiveValue;

	int findNearestTerminal(WeightMap);
};

#endif /* STEINERSOLVER_H_ */
