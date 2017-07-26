/*
 * dijkstra.h
 *
 * 	\brief     5. und 7. Aufgabe
 *  \details   Eigene Dijkstra Implementierung.
 *  \author    Christopher Wyczisk
 *  \date      20.07.2017
 */

#include "dijkstra.h"

/**
 * \brief Comparator fuer den Heap.
 */
struct comparator{
	bool operator()(const pair<int, int> ecke1, const pair<int, int> ecke2) const{
		bool comp = ecke1.second > ecke2.second;
		return comp;
	}
};

// helper
boost::heap::fibonacci_heap<pair<int, int>, boost::heap::compare<comparator>> heap;

/**
 * \brief Konstruktor.
 * 
 * \param gewichteMap Vector of the weights for the given graph
 * \param ecken
 * \param anzahlEckenGlobal
 */
dijkstra::dijkstra(vector<int> gewichtMap, vector<std::pair<int, int>> ecken, unsigned int anzahlEckenGlobalIn) {
	anzahlEckenGlobal = anzahlEckenGlobalIn;
	sortierteEcken.resize(anzahlEckenGlobal);
	for (unsigned int i = 0; i < ecken.size(); i++){
		sortierteEcken.at(ecken.at(i).first).push_back(std::make_pair(ecken.at(i).second, gewichtMap.at(i)));
		sortierteEcken.at(ecken.at(i).second).push_back(std::make_pair(ecken.at(i).first, gewichtMap.at(i)));
	}
}

/**
 * \brief Berechnet den Keurzesten laengsten Pfad vom Startknoten aus.
 * \param Startknoten
 */
vector<int> dijkstra::berechneKuerzestenPfad(int startKnoten) {
	initialisiere(startKnoten);
	int aktuellerKnoten = startKnoten;
	int aktuelleDistanze = 0;

	// init heap
	heap.push(std::make_pair(startKnoten, 0));

	// hauptschleife
	while(!heap.empty()){
		if (!adjanzen.at(heap.top().first)){
			aktuellerKnoten = heap.top().first;
			aktuelleDistanze = heap.top().second;
			heap.pop();
			// update gewichte
			vector<pair<int, int>> adjEcken = sortierteEcken.at(aktuellerKnoten);
			updateGewichte(adjEcken, aktuelleDistanze, aktuellerKnoten);
			adjanzen.at(aktuellerKnoten) = true;
		} else {
			heap.pop();
		}
	}
	return gewichte;
}

void dijkstra::initialisiere(int startKnoten) {
	for (unsigned int i = 0; i < anzahlEckenGlobal; i++) {
		gewichte.push_back(INT_MAX);
		vorgaenger.push_back(-1);
		adjanzen.push_back(false);
	}
	gewichte[startKnoten] = 0;
	vorgaenger[startKnoten] = startKnoten;
}

/**
 * \brief Hier updaten wir die Gewichte.
 */
void dijkstra::updateGewichte(vector<pair<int, int>> adjEcken, int aktuelleDistanze, int aktuelleEcke) {
	for (const pair<int, int> ecke : adjEcken) {
		int nachbar = ecke.first;
		int aktuell = ecke.second;
		
		if (!adjanzen.at(nachbar)) {
			heap.push(std::make_pair(nachbar, aktuelleDistanze + aktuell));
		}
		
		int alternative = aktuelleDistanze + aktuell;
		if(alternative < gewichte[nachbar]) {
			gewichte[nachbar] = alternative;
			vorgaenger[nachbar] = aktuelleEcke;
		}
	}
}
