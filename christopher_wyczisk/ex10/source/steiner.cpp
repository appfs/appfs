/*
 * steiner.cpp
 *
 * 	\brief     10. Aufgabe
 *  \details   Steinerklasse
 *  \author    Christopher Wyczisk
 *  \date      20.07.2017
 */

#include "../header/steiner.h"

#include <iostream>
#include <climits>
#include <algorithm>

/**
 * \brief Konstruktor.
 * 
 * \param vector<int> terminal
 */
steiner::steiner(vector<int> terminal) {
	terminals = new vector<int>(terminal);
	objektiverWert = 0;
}

steiner::~steiner() {
	delete terminals;
}

/**
 * \brief Loest das Steinerproblem.
 */
vector<pair<int, int>> steiner::loesen(vector<vector<pair<int, int>>>& ecken, unsigned int anzahlEckenGlobal, unsigned int startknoten) {
	vector<pair<int, int>> ergebnisse;
	dijkstra dijkstra(ecken, anzahlEckenGlobal);
	
	vector<bool> adj;
	vector<int> lokaleGewichte;
	vector<int> vorgaenger;
	
	for(int i = 0; i < anzahlEckenGlobal + 1; i++) {
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
	
	while(!terminals -> empty()) {
		dijkstra.berechneKuerzestenPfad(startknoten, lokaleGewichte, vorgaenger);

		int terminal = nextTerminal(lokaleGewichte);
		terminals -> erase(std::find(terminals -> begin(), terminals -> end(), terminal));
		
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
 * \brief Getter fuer den objektivenWert
 */
int steiner::gibObjektivenWert(){
	return objektiverWert;
}

/**
 * \brief berechnet das Terminal mit geringsten Gewchten.
 */
int steiner::nextTerminal(vector<int> gewichte) {
	int terminalEcke = 0;
	int terminalEckeGewicht = INT_MAX;
	
	//  Evenetuell waere es effizienter die Termals entsprechend ihres Gewichts zu sortieren. Leider finde ich gerade keine passende Comperatormethode.
	for(int i = 0; i < terminals -> size(); i++) {
		int neueTerminalEcke = terminals -> at(i);
		
		if(gewichte[neueTerminalEcke] < terminalEckeGewicht){
			terminalEckeGewicht = gewichte[neueTerminalEcke];
			terminalEcke = neueTerminalEcke;
		}
	}
	
	return terminalEcke;
}
