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

class GraphChecker {

private:
	Edges edges;
	int nodeCount;

public:
	char isConnected();
	char hasCycle();
	GraphChecker(Edges edges, int nodeCount);
	virtual ~GraphChecker();
};

#endif /* C_GRAPHCHECKER_H_ */
