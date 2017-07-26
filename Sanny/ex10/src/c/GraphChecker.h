/*
 * GraphChecker.h
 *
 *  Created on: 08.07.2017
 *      Author: alesan
 */

#ifndef C_GRAPHCHECKER_H_
#define C_GRAPHCHECKER_H_

#include <vector>
#include <queue>

using Edge = std::pair<int, int >;
using Edges = std::vector<Edge >;
using Adjacent = std::pair<int, int >;
using Adjacents = std::vector<Adjacent >;
using AdjacentsMap = std::vector<Adjacents >;
using Nodes = std::vector<int >;
using NodesToVisit = std::queue<int >;

class GraphChecker {

private:
	Edges edges;
	Nodes nodes;
	Nodes terminals;

	bool hasCycle(int node, char* visited, int parent, AdjacentsMap adjacentsMap);
	void getAdjacents(int node, Adjacents* adjs);
	AdjacentsMap makeAdjacentsMap();

public:
	bool isConnected();
	bool hasCycle();
	bool containsAllTerminals();
	GraphChecker(Edges edges, Nodes nodes, Nodes terminals);
	virtual ~GraphChecker();
};

#endif /* C_GRAPHCHECKER_H_ */
