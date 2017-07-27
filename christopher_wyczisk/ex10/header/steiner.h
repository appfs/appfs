/*
 * steiner.h
 *
 * 	\brief     10. Aufgabe
 *  \details   Steinerheader
 *  \author    Christopher Wyczisk
 *  \date      20.07.2017
 */

#ifndef STEINER_H_
#define STEINER_H_

#include "dijkstra.h"
#include <iostream>
#include <climits>
#include <algorithm>

class steiner {
	public:
		steiner(std::vector<int>);
		~steiner();
		vector<pair<int, int>> loesen(vector<vector<pair<int, int>>>& ecken, unsigned int anzahlEckenGlobal, unsigned int startknoten);
		int gibObjektivenWert();
	
	private:
		vector<pair<int, int>> berechneKanten(dijkstra dijkstra, unsigned int startknoten, vector<bool> adj, vector<int> lokaleGewichte, vector<int> vorgaenger);
		std::vector<int>* terminals;
		int objektiverWert;
		int nextTerminal(vector<int> gewichte);
};

#endif
