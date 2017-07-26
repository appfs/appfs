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
		vector<int> berechneKuerzestenPfad(int startKnoten);
	
	private:
		void updateGewichte(vector<pair<int, int>> adjEcken, int aktuelleDistanze, int aktuelleEcke);
		void initialisiere(int startKnoten);
		vector<std::vector<pair<int, int>>> sortierteEcken;
		unsigned int anzahlEckenGlobal;
		vector<int> gewichte;
		vector<int> vorgaenger;
		vector<bool> adjanzen;
};

#endif
