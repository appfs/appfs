/**
 *	main.cpp
 *
 * 	\brief     8. und 9. Aufgabe
 *  \details   Main. Loest das Steinergraph Problem.
 *  \author    Christopher Wyczisk
 *  \date      20.07.2017
 */

#include "../header/steiner.h"
#include "../header/kommunikator.h"

int main(int argc, char* argv[]) {
	kommunikator kommunikator;
	
	vector<int> startknoten = kommunikator.startknotenVonKonsole(argc, argv);
	if(startknoten.size() == 0) {
		return EXIT_FAILURE;
	}

	ifstream file;
	bool fileEingelesen = kommunikator.graphEinlesen(argc, argv, file);
	bool erstellt = kommunikator.graphErstellen(file);

	vector<pair<int, int>> ergebnisse;
	int objektiverWert = INT_MAX;
	int gewaehlerStartknoten;

	for(int knoten : startknoten) {
		steiner steinerAlgo(kommunikator.gewichte,  kommunikator.kanten,  kommunikator.anzahlEckenGlobal);
		vector<pair<int, int>> kanten = steinerAlgo.loesen(knoten);
		if (steinerAlgo.gibObjektivenWert() < objektiverWert) {
			objektiverWert = steinerAlgo.gibObjektivenWert();
			ergebnisse = kanten;
			gewaehlerStartknoten = knoten;
		}
	}
	
	kommunikator.ergebnisAusgabe(ergebnisse, objektiverWert, gewaehlerStartknoten);

	return EXIT_SUCCESS;
}