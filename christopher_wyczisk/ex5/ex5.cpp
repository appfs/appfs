/*
 * ex5.cpp
 *
 * 	\brief     5. Aufgabe
 *  \details   Liesst eine .gph Datei ein und berechnet den laengsten kuerzesten pfad von jeder Ecke zum Root des Graphen.
 *  \author    Christopher Wyczisk
 *  \date      08.06.2017
 */

#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/named_function_params.hpp>
#include <boost/timer/timer.hpp>

using namespace std;
using namespace boost::timer;

// helper
vector<pair<int, int>> kanten;
vector<int> gewichte;
unsigned int anzahlEckenGlobal;

/**
 *\typedef Adjancen des Graphen
 */
typedef boost::adjacency_list<boost::listS, boost::vecS,
	boost::undirectedS, boost::no_property,
	boost::property<boost::edge_weight_t, int>> graph;

/**
 * \typedef Die Informationen ueber die Ecken des Graphen
 */
typedef boost::graph_traits <graph>::vertex_descriptor eckenDto;

/**
 * Einlesen der Graph Datei und Fehlerbehandlung.
 * 
 * \param argc
 * \param argv
 * \param infile input Graph-File
 */
bool graphEinlesen(int argc, char** argv, ifstream& file) {
	if(argc <= 1) {
		cout << "Sie haben keinen Pfad zu einer .gph Datei angegeben." << endl;
		return false;
	} else {
		file.open(argv[1], ios::in);
	}

	if (!file) {
		cout << "Die von Ihnen angegebene Datei konnte nicht geoeffnet werden." << endl;
		return false;
	}
	
	return true;
}

/**
 * Konsolen-Ausgabe der Anzahl an Ecken und des Gewichts.
 * 
 * \param anzahlEcken
 * \param laegsterKuerzesterPfadGewicht
 */
void ergebnisAusgabe(int anzahlEcken, int laegsterKuerzesterPfadGewicht) {
	cout << "RESULT VERTEX " << anzahlEcken << endl;
	cout << "RESULT DIST " << laegsterKuerzesterPfadGewicht << endl;
}

/**
 * Erstellt den Graphen.
 * 
 * \param file
 */
bool graphErstellen(ifstream& file) {
	stringstream str;
	string zeile;
	int anzahlKanten;
	int start;
	int end;
	int gewicht;

	if (getline(file, zeile)) {
		str.str(zeile);
		str >> anzahlEckenGlobal >> anzahlKanten;
		anzahlEckenGlobal++;
	} else {
		cerr << "Das .gph File ist leer." << endl;
		return false;
	}

	while (getline(file, zeile)) {
		str.clear();
		str.str(zeile);
		str >> start >> end >> gewicht;
		kanten.push_back(make_pair(start, end));
		gewichte.push_back(gewicht);
	}
	
	return true;
}

/**
 * Berechnet das Gewicht des longest-shortest Pfad.
 * 
 * \param graph der graph
 * \param anzahlEcken
 * \param laengsterKuerzesterPfadGewicht
 */
void berechneLongesteShortesWeigth(graph graph, int& anzahlEcken, int& laegsterKuerzesterPfadGewicht) {
	vector<eckenDto> pfadRichtungen(anzahlEckenGlobal);
	vector<int> gewichteList(anzahlEckenGlobal);

	boost::dijkstra_shortest_paths(
		graph, 1,
		boost::predecessor_map(	boost::make_iterator_property_map(pfadRichtungen.begin(), get(boost::vertex_index, graph)))
			.distance_map(
					boost::make_iterator_property_map(gewichteList.begin(), get(boost::vertex_index, graph
					))));

	// berechnet das Gewicht
	for(int i = 1; i < anzahlEckenGlobal; i++) {
		if (gewichteList[i + 1] > laegsterKuerzesterPfadGewicht) {
			laegsterKuerzesterPfadGewicht = gewichteList[i + 1];
			anzahlEcken = i + 1;
		}
	}
}

/**
 * Main.
 */
int main(int argc, char* argv[]) {
	ifstream file;
	bool fileEingelesen = graphEinlesen(argc, argv, file);
	
	boost::timer::cpu_timer t;
	if(fileEingelesen) {
		//erstelle den Graph
		bool erstellt = graphErstellen(file);
		if(erstellt) {
			graph graph{kanten.begin(), kanten.end(), gewichte.begin(), anzahlEckenGlobal};
		
			// berechnen des gewichts des longesten shortesten pfad
			int laegsterKuerzesterPfadGewicht = -1;
			int anzahlEcken = -1;
			berechneLongesteShortesWeigth(graph, anzahlEcken, laegsterKuerzesterPfadGewicht);
		
			boost::timer::cpu_times zeit = t.elapsed();
			
			ergebnisAusgabe(anzahlEcken, laegsterKuerzesterPfadGewicht);
			std::cout << "WALL-CLOCK " << zeit.wall / 1e9 << "s" << std::endl;
			std::cout << "USER TIME " << zeit.user / 1e9 << "s" << std::endl;
		}
	}
	return 0;
}