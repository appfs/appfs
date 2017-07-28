/**
 *	graph.cpp
 *
 * 	\brief     10. Aufgabe
 *  \details   Helper zum pruefen des Baums.
 *  \author    Christopher Wyczisk
 *  \date      20.07.2017
 */
#include "../header/graph.h"

/**
 * \brief Konstruktor.
 * 
 * \param vector<pair<int, int>> kanten
 * \param int anzahlEcken
 */
graph::graph(vector<pair<int, int>> kantenIn, int anzahlEckenIn) {
	kanten.resize(anzahlEckenIn + 1);
	effizienteEcken.resize(anzahlEckenIn + 1);
	anzahlErreichbareKnoten.resize(anzahlEckenIn + 1, 1);

	for(int i = 0; i < kantenIn.size(); i++) {
		pair<int, int> kanteIn = kantenIn.at(i);
		
		kanten.at(kanteIn.first).push_back(kanteIn.second);
		kanten.at(kanteIn.second).push_back(kanteIn.first);
		effizienteEcken.at(kanteIn.first) = true;
		effizienteEcken.at(kanteIn.second) = true;
		
		disjunkteVektorErweiterung(ecken, kanteIn.first);
		disjunkteVektorErweiterung(ecken, kanteIn.second);
	}
	
	sucheEffizienteEcken();
}

/**
 * \brief Helpermethode die dem Vektor ein Element zugfuegt, wenn dieser dieses Element noch nicht beinhaltet.
 */
void graph::disjunkteVektorErweiterung(vector<int>& vektor, int neuerKandidat) {
	bool unbekanterKandidat = find(vektor.begin(), vektor.end(), neuerKandidat) == vektor.end();
	if(unbekanterKandidat) {
		vektor.push_back(neuerKandidat);
	}
}

/**
 * \brief Prueft ob der Graph vollstaendig ist.
 * 
 * \param vector<int>* terminals
 */
bool graph::vollstaendig(vector<int>* terminals) {
	bool vollstaendig = true;
	
	for(int i = 0; i < terminals -> size(); i++) {
		if(!effizienteEcken.at(terminals -> at(i))) {
			vollstaendig = false;
		}
	}
	
	return vollstaendig;
}

/**
 * \brief prueft, ob der Graph Kreise enthaelt.
 */
bool graph::keineZykel() {
	bool hatZykel = find(anzahlErreichbareKnoten.begin(), anzahlErreichbareKnoten.end(), 2) != anzahlErreichbareKnoten.end();
	return !hatZykel;
}

/**
 * Prueft ob der Graph zusammenhaengend ist.
 */
bool graph::zusammenhaengend() {
	bool unzusammenhaengend = find(anzahlErreichbareKnoten.begin(), anzahlErreichbareKnoten.end(), 0) != anzahlErreichbareKnoten.end();
	return !unzusammenhaengend;
}

/**
 * \brief Berechnet die Effizienten Ecken.
 */
void graph::sucheEffizienteEcken() {
	setzeErreichbarkeitAufInit();
	vector<int> knotenErreichbar;
	knotenErreichbar.push_back(ecken.at(0));

	while(knotenErreichbar.size() != 0) {
		iterationSuche(knotenErreichbar);
	}
}

/**
 * \brief Setzte die Erreichbarkeit von Knoten auf 0.
 */
void graph::setzeErreichbarkeitAufInit() {
	for(int i = 0; i < ecken.size(); i++) {
		int ecke = ecken.at(i);
		anzahlErreichbareKnoten.at(ecke) = 0;
	}
}

/**
 * \brief Helper fuer sucheEffizienteEcken.
 */
void graph::iterationSuche(vector<int>& knotenErreichbar) {
	int aktueller = knotenErreichbar.at(0);
	knotenErreichbar.erase(knotenErreichbar.begin());
	anzahlErreichbareKnoten.at(aktueller)++;
	
	for(int i = 0; i < kanten.at(aktueller).size(); i++) {
		int knoten = kanten.at(aktueller).at(i);
		knotenErreichbar.push_back(knoten);
		kanten.at(knoten).erase(
			find(kanten.at(knoten).begin(), kanten.at(knoten).end(), aktueller)
		);
	}
}