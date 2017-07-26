/*
 * dijkstra.h
 *
 * 	\brief     10. Aufgabe
 *  \details   Headerfile der Dijkstra Impelementierung.
 *  \author    Christopher Wyczisk
 *  \date      20.07.2017
 */

#ifndef DIJKSTRA_H_
#define DIJKSTRA_H_

#include <vector>
#include <climits>
#include <iostream>
#include <boost/heap/fibonacci_heap.hpp>
#include <exception>
#include "kommunikator.h"

using namespace std;

// klassenheader
class dijkstra {
	public:
		dijkstra(vector<vector<pair<int, int>>> ecken, unsigned int anzahlEckenGlobal);
		void berechneKuerzestenPfad(int startknoten, vector<int>& gewichte, vector<int>& vorgaenger);
		int initialisierseEcken(int startEcke, int endEcke);
	
	private:
		vector<pair<int, int>> updateGewichte(vector<bool>& adjanzen, vector<pair<int, int>> adjEcken, int aktuelleDistanze, int aktuelleEcke, vector<int>& gewichte, vector<int>& vorgaenger);
		vector<bool> initialisiere(int startknoten, vector<int>& gewichte, vector<int>& vorgaenger);
		vector<vector<pair<int, int>>> sortierteKanten;
		unsigned int anzahlEckenGlobal;
};

#endif