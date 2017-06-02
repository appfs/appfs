#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <vector>

using namespace std;



int main(int argc, char* argv[]){

    string line;
    string lnbckp;
	int linenumber = 0;
	int category =0;
	double value = 0.0;
	int errors = 0;
	int totLines = 0;


    if(argc < 2) {
        cerr << "provide at least one argument" << endl;
        return 1;
        }
    cout << "* trying to open and read: " << argv[1] << endl;

    ifstream f (argv[1]);

    vector<double> values1;
    vector<double> values2;



    while(getline(f, line)) {

    	lnbckp = line;
    	totLines++;
		size_t posr;
		if((posr = line.find("#")) == 0){
			errors++;
			continue;
		}

		if(line[0] == '\n'){
			errors++;
			continue;
		}

		std::string delimiter = ";";

		size_t pos = 0;
		std::string token;

		try{
			pos = line.find(delimiter);
			linenumber = std::stoi(line.substr(0, pos));
			line.erase(0, pos + delimiter.length());

			pos = line.find(delimiter);
			category = std::stoi(line.substr(0, pos));
			line.erase(0, pos + delimiter.length());

			value = std::stod(line);

		}catch(...){
			errors++;
			continue;
		}

		if(linenumber <0){
			errors++;
			continue;
		}
		if(!(0 < category < 3)){
			errors++;
			continue;
		}
		if(value < 0.0){
			errors++;
			continue;
		}
		if(isnan(value)){
			errors++;
			continue;
		}


		if(category==1){
			values1.push_back(log(value));
		}
		else if(category==2){
			values2.push_back(log(value));
		}
    }

	f.close();

	double sum1 = 0;
	double sum2 = 0;

	for( int i=0; i < values1.size(); i++){
		sum1 += values1[i];
	}

	for( int i=0; i < values2.size(); i++){
		sum2 += values2[i];
	}

	std::cout << "File: " << argv[1] << " with " << totLines << " lines." << std::endl;
	std::cout << "Valid values Loc1: " << values1.size() << " with GeoMean: " << exp(sum1/values1.size()) << std::endl;
	std::cout << "Valid values Loc2: " << values2.size() << " with GeoMean: " << exp(sum2/values2.size()) << std::endl;
	std::cout << "Errors occurred: " << errors << std::endl;

	/*OUTPUT SHOULD BE:

	File:	ex1.dat	with	100001235	lines
	Valid	values	Loc1:	50004598	with	GeoMean:	36.7817
	Valid	values	Loc2:	49994703	with	GeoMean:	36.7825

    Should be 1934 errors

	*/

	return 0;

}





