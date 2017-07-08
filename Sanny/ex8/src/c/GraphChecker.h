/*
 * GraphChecker.h
 *
 *  Created on: 08.07.2017
 *      Author: alesan
 */

#ifndef C_GRAPHCHECKER_H_
#define C_GRAPHCHECKER_H_

#include <vector>

using Edge = std::pair<int, int >;
using Edges = std::vector<Edge >;
using Adjacent = std::pair<int, int >;
using Adjacents = std::vector<Adjacent >;
using Nodes = std::vector<int >;

class GraphChecker {

private:
	Edges edges;
	Nodes nodes;
	char hasCycle(int node, char* visited, int parent);

public:
	char isConnected();
	char hasCycle();
	GraphChecker(Edges edges, Nodes nodes);
	virtual ~GraphChecker();
};

#endif /* C_GRAPHCHECKER_H_ */
