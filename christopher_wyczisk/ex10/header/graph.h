/**
 *	graph.h
 *
 * 	\brief     10. Aufgabe
 *  \details   Helper zum pruefen des Baums.
 *  \author    Christopher Wyczisk
 *  \date      20.07.2017
 */
#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

class graph {
	public:
	    graph(vector<pair<int, int>> kanten, int anzahlEcken);
		bool vollstaendig(std::vector<int>* hundertTerminals);
		bool keineZykel();
		bool zusammenhaengend();
		
	private:
		void disjunkteVektorErweiterung(vector<int>& vektor, int neuerKandidat);
		void setzeErreichbarkeitAufInit();
		void iterationSuche(vector<int>& knotenErreichbar);
		vector<int> anzahlErreichbareKnoten;
		vector<vector<int>> kanten;
		vector<bool> effizienteEcken;
		vector<int> ecken;
		void sucheEffizienteEcken();
};

#endif