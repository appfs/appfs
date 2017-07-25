/*
 * steiner.cpp
 *
 * 	\brief     8. und 9. Aufgabe
 *  \details   Steinerklasse
 *  \author    Christopher Wyczisk
 *  \date      20.07.2017
 */

#include "../header/steiner.h"

/**
 * \brief Konstruktor.
 * 
 * \param vector<int> gewichte
 * \param vector<pair<int, int>> ecken
 * \param unsigned int anzahlEckenGlobal
 */
steiner::steiner(vector<int> gewichteIn, vector<pair<int, int>> eckenIn, unsigned int anzahlEckenGlobalIn) {
	gewichte = gewichteIn;
	ecken = eckenIn;
	anzahlEckenGlobal = anzahlEckenGlobalIn;
	terminals = primzahlen();
	objektiverWert = 0;
}

/**
 * \brief Loest das Steinerproblem.
 */
vector<pair<int, int>> steiner::loesen(unsigned int startknoten) {
	vector<pair<int, int>> ergebnisse;
	dijkstra dijkstra(gewichte, ecken, anzahlEckenGlobal);
	
	vector<bool> adj;
	vector<int> lokaleGewichte;
	vector<int> vorgaenger;
	
	for(int i = 0; i < anzahlEckenGlobal; i++) {
		adj.push_back(false);
		vorgaenger.push_back(-1);
		lokaleGewichte.push_back(INT_MAX);
	}
	
	ergebnisse = berechneKanten(dijkstra, startknoten, adj, lokaleGewichte, vorgaenger);
	return ergebnisse;
}

/**
 * \brief Helper fuer loesen.
 */
vector<pair<int, int>> steiner::berechneKanten(dijkstra dijkstra, unsigned int startknoten, vector<bool> adj, 	vector<int> lokaleGewichte, vector<int> vorgaenger) {
	vector<pair<int, int>> kanten;
	
	while(!terminals.empty()) {
		dijkstra.berechneKuerzestenPfad(startknoten, lokaleGewichte, vorgaenger);

		int terminal = nextTerminal(lokaleGewichte);
		terminals.erase(std::find(terminals.begin(), terminals.end(), terminal));
		
		while(terminal != startknoten && !adj[terminal]) {
			kanten.push_back(std::make_pair(terminal, vorgaenger[terminal]));
			objektiverWert += dijkstra.initialisierseEcken(terminal, vorgaenger[terminal]);
			dijkstra.initialisierseEcken(vorgaenger[terminal], terminal);

			adj[terminal] = true;
			terminal = vorgaenger[terminal];
		}
	}
	
	return kanten;
}

/**
 * \brief berechnet das Terminal mit geringsten Gewchten.
 */
int steiner::nextTerminal(vector<int> gewichte) {
	int terminalEcke = 0;
	int terminalEckeGewicht = INT_MAX;
	
	//  Evenetuell waere es effizienter die Termals entsprechend ihres Gewichts zu sortieren. Leider finde ich gerade keine passende Comperatormethode.
	for(int i = 0; i < terminals.size(); i++) {
		int neueTerminalEcke = terminals[i];
		
		if(gewichte[neueTerminalEcke] < terminalEckeGewicht){
			terminalEckeGewicht = gewichte[neueTerminalEcke];
			terminalEcke = neueTerminalEcke;
		}
	}
	
	return terminalEcke;
}

/**
 * \brief Helper von primzahlen().
 */
bool steiner::hatTeiler(const std::vector<int>& primzahlen, int zahl) {
	bool hatTeiler = false;
	int i = 0;
	
	while(i < primzahlen.size() && !hatTeiler) {
		int primzahl = primzahlen[i];
		hatTeiler = ((zahl % primzahl) == 0 || primzahl / 2 >= zahl);
		i++;
	}
	
	return hatTeiler;
}

/*
 * \brief berechnet alle relevanten primzahlen bis AnzahlEkcneGlobal.
 */
vector<int> steiner::primzahlen() {
	vector<int> primzahlen;

	for(int i = 2; i < anzahlEckenGlobal; i++) {
		if(!hatTeiler(primzahlen, i)) {
			primzahlen.push_back(i);
		}
	}
	
	return primzahlen;
}

/**
 * \brief Getter fuer den objektivenWert
 */
int steiner::gibObjektivenWert(){
	return objektiverWert;
}