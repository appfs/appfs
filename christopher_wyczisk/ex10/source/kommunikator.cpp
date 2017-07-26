/*
 *  kommunikator.cpp
 *
 * 	\brief     10. Aufgabe
 *  \details   Kuemmert sich um die Auswertung und wWitergabe der vom Anwender eingegebenen Daten.
 *  \author    Christopher Wyczisk
 *  \date      20.07.2017
 */

#include "../header/kommunikator.h"

kommunikator::kommunikator() {
	kanten = new vector<vector<pair<int, int>> >();
    vector<int> gewichte;
    unsigned int anzahlEckenGlobal;
	boost::timer::cpu_timer cpu_timer;
	boost::timer::cpu_timer wall_timer;
	boost::timer::cpu_times wall_time;
}

void kommunikator::starteZeitmessung() {
	wall_time = wall_timer.elapsed();
}

void kommunikator::ausgabeDerLaufzeit() {
	boost::timer::cpu_times cpu_time = cpu_timer.elapsed();
	cout << "TIME: " << (cpu_time.system + cpu_time.user) * 1e-9 << " seconds" << endl;
	cout << "WALL: " << wall_time.wall * 1e-9 <<  " seconds" << endl;
}

int kommunikator::anzahlThreads(char* argvThreads) {
	int anzahlThreads;
	stringstream ss(argvThreads);
	ss >>anzahlThreads;
	
	return anzahlThreads;
}

void kommunikator::ausgabe(int ergebniss, vector<pair<int, int>> kanten) {
	cout << "Baumlaenge: " << ergebniss << endl;
	cout << "Baumkanten: ";
	for(int i = 0; i < kanten.size(); i++){
		if(i == kanten.size() -1){
			cout << "(" << kanten.at(i).first << "," << kanten.at(i).second << ")" << endl;
		}else {
			cout << "(" << kanten.at(i).first << "," << kanten.at(i).second << ") ";
		}
	}
}

/**
 * Einlesen der Graph Datei und Fehlerbehandlung.
 * 
 * \param argc
 * \param argv
 * \param infile input Graph-File
 */
bool kommunikator::graphEinlesen(int argc, char** argv, ifstream& file) {
	if(argc <= 1) {
		cout << "Sie haben keinen Pfad zu einer .gph Datei angegeben." << endl;
		return false;
	} else {
		file.open(argv[2], ios::in);
	}

	if (!file) {
		cout << "Die von Ihnen angegebene Datei konnte nicht geoeffnet werden." << endl;
		return false;
	}
	
	return true;
}

/**
 * Erstellt den Graphen.
 * 
 * \param file
 */
bool kommunikator::graphErstellen(ifstream& file) {
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
	
	kanten -> resize(anzahlEckenGlobal + 1, vector<pair<int, int>>());

	while (getline(file, zeile)) {
		str.clear();
		str.str(zeile);
		str >> start >> end >> gewicht;
		//kanten.push_back(make_pair(start, end));
		gewichte.push_back(gewicht);
		
		kanten -> at(start).push_back(make_pair(end, gewicht));
		kanten -> at(end).push_back(make_pair(start, gewicht));
	}
	
	return true;
}

/**
 * \brief Liesst die Startknoten ein die beim starten des Programs als Argumente
 * der Form --start 1 2 3 4 mitgegeben wurden und gibt diese zurueck. Sollte kein
 * Startknoten angegeben worden sein, so wird der Knoten 2 als Default zurueckgegeben.
 */
/*vector<int> kommunikator::startknotenVonKonsole(int argc, char** argv) {
	vector<int> startknoten;
	options_description optionenCmd("");

	optionenCmd.add_options()
	    ("startknoten,start", value<vector<int> >()->multitoken(), "Startknoten")
		("pfadZurDatei,pfad", value<string>(), "Pfad zur .gph Datei");

	command_line_parser parser{argc, argv};
	parser.options(optionenCmd).allow_unregistered().style(
	      command_line_style::default_style |
	      command_line_style::allow_slash_for_short);
	parsed_options parseCmd = parser.run();

	variables_map vm;
	store(parseCmd, vm);
	notify(vm);

	if(vm.count("pfadZurDatei") == 0) {
		cerr << "Gibt bitte einen Pfad zu einer .gph Datei an." << endl;
		return startknoten;
	}

	if(vm.count("startknoten") == 0){ 
		cout << "Default Startknoten 2, erste Primzahl" << endl;
		startknoten.push_back(2);
	} else {
		startknoten = vm["startknoten"].as<vector<int> >();
	}
	
	return startknoten;
}*/

/**
 * \brief Schreibt die Ergebnisse in die Konsole.
 */
/*void kommunikator::ergebnisAusgabe(vector<pair<int, int>> ergebnis, int objektiverWert, int startknoten) {
	cout << "Der Minimaler Spannbaum ergebnit sich mit folgendem Startknoten: " << startknoten << endl;

	cout << "Hier die Kanten des Baums: " << endl;
	for(pair<int, int> kante : ergebnis) {
		cout << kante.first << " " << kante.second << endl;
	}

	cout << "Der objektive Wert des Baums ist " << objektiverWert << endl;
}*/