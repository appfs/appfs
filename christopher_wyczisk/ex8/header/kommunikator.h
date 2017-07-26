/*
 *  kommunikator.h
 *
 * 	\brief     8. und 9. Aufgabe
 *  \details   Kuemmert sich um die Auswertung und wWitergabe der vom Anwender eingegebenen Daten.
 *  \author    Christopher Wyczisk
 *  \date      20.07.2017
 */

#ifndef KOMMUNIKATOR_H_
#define KOMMUNIKATOR_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <boost/program_options.hpp>

using namespace std;
using namespace boost::program_options;

class kommunikator {
	public:
		bool graphEinlesen(int argc, char** argv, ifstream& file);
		bool graphErstellen(ifstream& file);
		vector<int> startknotenVonKonsole(int argc, char** argv);
		void ergebnisAusgabe(vector<pair<int, int>> result, int minObjValue, int minStartNode);
	    vector<pair<int, int>> kanten;
        vector<int> gewichte;
        unsigned int anzahlEckenGlobal;
};

#endif