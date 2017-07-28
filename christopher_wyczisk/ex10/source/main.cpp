/*
 *	main.cpp
 *
 * 	\brief     10. Aufgabe
 *  \details   Main. Loest das Steinergraph Problem.
 *  \author    Christopher Wyczisk
 *  \date      20.07.2017
 */
#include <iostream>
#include <boost/program_options.hpp>

#include "../header/steiner.h"
#include "../header/graph.h"
#include "../header/kommunikator.h"

using namespace std;

vector<std::pair<int, int>> ecken[100];
int ergebnisse[100];

/**
 * \brief Helper von primzahlen().
 */
bool hatTeiler(const std::vector<int>* primzahlen, int zahl) {
	bool hatTeiler = false;
	int i = 0;
	
	while(i < primzahlen -> size() && !hatTeiler) {
		int primzahl = primzahlen -> at(i);
		hatTeiler = ((zahl % primzahl) == 0 || primzahl / 2 >= zahl);
		i++;
	}
	
	return hatTeiler;
}

/**
 * \brief berechnet alle relevanten primzahlen bis AnzahlEkcneGlobal.
 */
void primzahlen(vector<int>* primzahlen, int anzahlEcken) {
	for(int i = 2; i < anzahlEcken; i++) {
		if(!hatTeiler(primzahlen, i)) {
			primzahlen -> push_back(i);
		}
	}
}

vector<int>* berechneHundertTerminals(vector<int>* terminals, kommunikator* kom) {
	vector<int>* hundertTerminals;
	primzahlen(terminals, kom -> anzahlEckenGlobal);
	if(terminals -> size() < 100) {
		hundertTerminals = terminals;
	} else {
		hundertTerminals = new vector<int>(terminals -> begin(), terminals -> begin() + 100);
	}
	return hundertTerminals;
}

/**
 * \brief berechnet unter nutzung von threads den index zu den knoten mit minimalen wert.
 */
int berechneMinimalenKnotenkey(kommunikator* kom, int anzahlThreads, vector<int>* terminals, vector<int>* hundertTerminals) {
	int minKey;
	int wert = INT_MAX;
	
	#pragma omp parallel for num_threads(anzahlThreads)
	for(int i = 0; i < hundertTerminals -> size(); i++) {
		steiner* steinerAlgo = new steiner(*terminals);
		ecken[i] = steinerAlgo -> loesen(*kom -> kanten, kom -> anzahlEckenGlobal, hundertTerminals -> at(i));
		ergebnisse[i] = steinerAlgo -> gibObjektivenWert();
		delete steinerAlgo;
	}

	for(int i = 0; i < hundertTerminals -> size(); i++) {
		if(ergebnisse[i] < wert){
			minKey = i;
			wert = ergebnisse[i];
		}
	}
	return minKey;
}

int main(int argc, char* argv[]) {
	kommunikator* kom = new kommunikator();
	int anzahlThreads = kom -> anzahlThreads(argv[1]);

	ifstream file;
	bool fileEingelesen = kom -> graphEinlesen(argc, argv, file);
	bool erstellt = kom -> graphErstellen(file);
	
	vector<int>* terminals = new vector<int>();
	vector<int>* hundertTerminals = berechneHundertTerminals(terminals, kom);
	
	kom -> starteZeitmessung();
	int minKey = berechneMinimalenKnotenkey(kom, anzahlThreads, terminals, hundertTerminals);

	graph g(ecken[minKey], kom -> anzahlEckenGlobal);
	if(!g.vollstaendig(hundertTerminals)) {
		cout << "Einige Terminals fehlen" << endl;
	}
	if(!g.keineZykel()){
		cout << "Kreise im Graph gefunden" << endl;
	}
	if(!g.zusammenhaengend()){
		cout << "Nicht zusammenhaengend" << endl;
	}

	//ausgabe
	kom -> ausgabe(ergebnisse[minKey], ecken[minKey]);

	kom -> ausgabeDerLaufzeit();
	delete hundertTerminals;
	delete kom;
	return EXIT_SUCCESS;
}