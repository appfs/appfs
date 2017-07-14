/*
 * Steiner.h
 *
 *  Created on: 06.07.2017
 *      Author: alesan
 */

#ifndef C_STEINER_H_
#define C_STEINER_H_

#include <algorithm>
#include "Dijkstra.h"

using Nodes = std::vector<int >;
using Terminals = std::vector<int >;

class Steiner {
private:
	int steinerWeight = 0;
	Nodes* nodesInTree;
	Edges* steinerEdges;
	void addToSteiner(Edge edge, unsigned int i, Weights* weights);

public:
	Steiner();
	virtual ~Steiner();
	void computeSteinerTree(int vertexCount, Edges* edges, Weights& weights, Terminals& terminals, int startnode);
	int getWeight();
	Edges getEdges();
	Nodes getNodes();
};

#endif /* C_STEINER_H_ */
