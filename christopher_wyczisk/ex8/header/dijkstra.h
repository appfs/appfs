/*
 * dijkstra.h
 *
 * 	\brief     5. und 7. Aufgabe
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

using namespace std;

// klassenheader
class dijkstra {
	public:
		dijkstra(vector<int> gewichteMap, vector<pair<int, int>> ecken, unsigned int anzahlEckenGlobal);
		vector<int> berechneKuerzestenPfad(int startKnoten, vector<int>& gewichte, vector<int>& vorgaenger);
		int initialisierseEcken(int startEcke, int endEcke);
		
	private:
		void updateGewichte(vector<pair<int, int>> adjEcken, int aktuelleDistanze, int aktuelleEcke, vector<int>& gewichte, vector<int>& vorgaenger);
		void initialisiere(int startKnoten, vector<int>& gewichte, vector<int>& vorgaenger);
		vector<std::vector<pair<int, int>>> sortierteEcken;
		unsigned int anzahlEckenGlobal;
		vector<bool> adjanzen;
};

#endif
