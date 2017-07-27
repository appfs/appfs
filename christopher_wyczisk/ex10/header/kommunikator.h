/*
 *  kommunikator.h
 *
 * 	\brief     10. Aufgabe
 *  \details   Kuemmert sich um die Auswertung und wWitergabe der vom Anwender eingegebenen Daten.
 *  \author    Christopher Wyczisk
 *  \date      20.07.2017
 */

#ifndef KOMMUNIKATOR_H_
#define KOMMUNIKATOR_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <boost/timer/timer.hpp>

using namespace std;

class kommunikator {
	public:
		kommunikator();
		void starteZeitmessung();
		void ausgabeDerLaufzeit();
		void ausgabe(int ergebnis, vector<pair<int, int>>);
		int anzahlThreads(char* argvThreads);
		bool graphEinlesen(int argc, char** argv, ifstream& file);
		bool graphErstellen(ifstream& file);
		vector<vector<pair<int, int>>>* kanten;
        vector<int> gewichte;
        unsigned int anzahlEckenGlobal;
	
	private:
		/*
		vector<int> startknotenVonKonsole(int argc, char** argv);
		void ergebnisAusgabe(vector<pair<int, int>> result, int minObjValue, int minStartNode);*/
        boost::timer::cpu_timer cpu_timer;
        boost::timer::cpu_timer wall_timer;
        boost::timer::cpu_times wall_time;
};

#endif