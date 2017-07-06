/*
 * Steiner.h
 *
 *  Created on: 06.07.2017
 *      Author: alesan
 */

#ifndef C_STEINER_H_
#define C_STEINER_H_

#include "Dijkstra.h"

using Edge = std::pair<int, int >;
using Edges = std::vector<Edge >;
using Weights = std::vector<double >;

class Steiner {
private:
	Dijkstra* dijsktra;

public:
	Steiner();
	virtual ~Steiner();
	unsigned int steiner(unsigned int vertexCount, Edges& edges, Weights& weights);
};

#endif /* C_STEINER_H_ */
