/**
 * steiner.h
 *
 * 	\brief     8. und 9. Aufgabe
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
		steiner(vector<int> gewichte, vector<pair<int, int>> ecken, unsigned int anzahlEckenGlobal);
		vector<pair<int, int>> loesen(unsigned int startknoten);
		int gibObjektivenWert();
	
	private:
		vector<int> primzahlen();
		vector<pair<int, int>> berechneKanten(dijkstra dijkstra, unsigned int startknoten, vector<bool> adj, 	vector<int> lokaleGewichte, vector<int> vorgaenger);
		bool hatTeiler(const std::vector<int>& ergebnisse, int i);
		int nextTerminal(vector<int> gewichte);
		int anzahlEckenGlobal;
		vector<pair<int, int>> ecken;
		vector<int> gewichte;
		std::vector<int> terminals;
		int objektiverWert;
};

#endif
