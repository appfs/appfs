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

#include "dijkstra.h"

class Steiner {
public:
	Steiner(int, Edges, WeightMap, std::vector<int>);
	Edges solveSteiner(unsigned int);
	int getObjectiveValue();

private:
	int numberOfVertices;
	Edges edges;
	WeightMap weights;
	std::vector<int> terminals;
	int objectiveValue;

	int findNearestTerminal(WeightMap);
};

#endif /* STEINER_H_ */
