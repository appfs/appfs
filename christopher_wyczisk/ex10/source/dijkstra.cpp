/*
 * dijkstra.h
 *
 * 	\brief     10. Aufgabe
 *  \details   Eigene Dijkstra Implementierung.
 *  \author    Christopher Wyczisk
 *  \date      20.07.2017
 */

#include "../header/dijkstra.h"

/**
 * \brief Comparator fuer den Heap.
 */
struct comparator{
	bool operator()(const pair<int, int> ecke1, const pair<int, int> ecke2) const{
		bool comp = ecke1.second > ecke2.second;
		return comp;
	}
};

/**
 * \brief Konstruktor.
 * 
 * \param gewichte
 * \param ecken
 * \param anzahlEckenGlobal
 */
dijkstra::dijkstra(vector<vector<pair<int, int>>> sortedEdges, unsigned int numberOfVertices) {
		sortierteKanten = sortedEdges;
		anzahlEckenGlobal = numberOfVertices;
}

/**
 * \brief Berechnet den Keurzesten laengsten Pfad vom Startknoten aus.
 * \param Startknoten
 */
void dijkstra::berechneKuerzestenPfad(int startknoten, vector<int>& gewichte, vector<int>& vorgaenger) {
	vector<bool> adjanzen = initialisiere(startknoten, gewichte, vorgaenger);
	int aktuellerKnoten = startknoten;
	int aktuelleDistanze = 0;
	
	// init heap
	boost::heap::fibonacci_heap<pair<int, int>, boost::heap::compare<comparator>> heap;
	heap.push(std::make_pair(startknoten, 0));

	while(!heap.empty()) {
		if (!adjanzen.at(heap.top().first)) {
			aktuellerKnoten = heap.top().first;
			aktuelleDistanze = heap.top().second;
			heap.pop();
			// update gewichte
			std::vector<pair<int, int>> adjEcken = sortierteKanten.at(aktuellerKnoten);
			vector<pair<int, int>> heapKnoten = updateGewichte(adjanzen, adjEcken, aktuelleDistanze, aktuellerKnoten, gewichte, vorgaenger);
			for(const pair<int, int> ecke: heapKnoten) {
				heap.push(std::make_pair(ecke.first, aktuelleDistanze + ecke.second));
			}
			adjanzen.at(aktuellerKnoten) = true;
		} else {
			heap.pop();
		}
	}
	//return gewichte;
}

vector<bool> dijkstra::initialisiere(int startKnoten, vector<int>& gewichte, vector<int>& vorgaenger) {
	vector<bool> adjanzen;
	for (unsigned int i = 0; i < anzahlEckenGlobal + 1; i++) {
		//gewichte.push_back(INT_MAX);
		//vorgaenger.push_back(-1);
		adjanzen.push_back(false);
	}
	gewichte[startKnoten] = 0;
	vorgaenger[startKnoten] = startKnoten;
	return adjanzen;
}

/**
 * \brief Hier updaten wir die Gewichte.
 */
vector<pair<int, int>> dijkstra::updateGewichte(vector<bool>& adjanzen, vector<pair<int, int>> adjEcken, int aktuelleDistanze, int aktuelleEcke, vector<int>& gewichte, vector<int>& vorgaenger) {
	vector<pair<int, int>> heapKnoten;
	for (const pair<int, int> ecke : adjEcken) {
		int nachbar = ecke.first;
		int aktuell = ecke.second;
		
		if (!adjanzen.at(nachbar)) {
			heapKnoten.push_back(ecke);
		}
		
		int alternative = aktuelleDistanze + aktuell;
		if(alternative < gewichte[nachbar]) {
			gewichte[nachbar] = alternative;
			vorgaenger[nachbar] = aktuelleEcke;
		}
	}
	
	return heapKnoten;
}


/**
 * \brief Setzt Ecken Gewicht auf Null und gibt das alte Gewicht zurueck.
 */
int dijkstra::initialisierseEcken(int startEcke, int endEcke) {
	std::vector<pair<int, int>>& ecken = sortierteKanten[startEcke];
	for(pair<int, int>& ecke : ecken) {
		if(ecke.first == endEcke) {
			int gewicht = ecke.second;
			ecke.second = 0;
			return gewicht;
		}
	}
	return 0;
}