#include <fstream>
#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <sstream>

using namespace std;


/**
 * Prueft, ob die Zeile ausgewertet werden soll
 */
bool lineIsValid(string line){
	if (line.empty()){
		return false;
	}
	if (line[0] == '#'){
		return false;
	}
	return true;
}

/**
 * ermittelt die Location einer Zeile
 */
int getLocation(string line){
	int location = 0;
	size_t pos = line.find_first_of(';');
	stringstream loc;
	if (pos != string::npos){
		line = line.substr(pos + 2);
		pos = line.find_first_of(';');
		if (pos != string::npos){
			loc.str(line.substr(0, pos));
			loc >> location;
			loc.clear();
		}
	}
	return location;
}

/*
 * ermittelt den Wert einer Zeile
 */
double getValue(string line){
	double value = 0;
	size_t pos = line.find_first_of(';');
	stringstream val;
	if (pos != string::npos){
		line = line.substr(pos + 2);
		pos = line.find_first_of(';');
		if (pos != string::npos){
			val.str(line.substr(pos + 2));
			val >> value;
			val.clear();
		}
	}
	return value;
}


/*
 * berechnet das geometrische Mittel eines Vektors
 */
double geoMean(vector<double> vec, int length){
	double sum = 0.;
	for (int i = 0; i < length; ++i) {
		sum += log(vec.at(i));
	}
	sum /= length;
	return exp(sum);
}




int main(int argc, char* argv[]){
	ifstream infile;

	if(argc <=1){
		infile.open("ex1.dat", ios::in);
	}
	else {
		infile.open(argv[1], ios::in);
	}

	if (!infile){
		cout << "Datei konnte nicht geöffnet werden" << endl;
		return 1;
	}

	string line;
	int lineCount=0;
	vector<double> loc1;
	vector<double> loc2;


	while (getline(infile, line)){
		lineCount++;
		if (lineIsValid(line)){
			int location = getLocation(line);
			double value = getValue(line);
			if (value > 0){
				if (location == 1){
					loc1.push_back(value);
				}else if (location == 2) {
					loc2.push_back(value);
				}
			}
		}
	}

	int sizeLoc1 = loc1.size();
	int sizeLoc2 = loc2.size();


	cout << "File: ex1.dat with " << lineCount << " lines" << endl;
	cout << "Valid values Loc1: " << sizeLoc1 << " with GeoMean: " << geoMean(loc1, sizeLoc1) << endl;
	cout << "Valid values Loc2: " << sizeLoc2 << " with GeoMean: " << geoMean(loc2, sizeLoc2) << endl;

	return 1;
 }


