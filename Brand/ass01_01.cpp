#include <iostream>
#include <fstream>
#include <string>
#include <math.h>


using namespace std;



int main(int argc, char* argv[]) {
    string line;
	int linenumber = 0;
	int category =0;
	double value = 0.0;
	int errors = 0;


    if(argc < 2) {
        cerr << "provide at least one argument" << endl;
        return 1;
        }
    cout << "* trying to open and read: " << argv[1] << endl;

    ifstream f (argv[1]);

	double absValOne = 0.0;

	double aV1_1 = 0.0;
	double aV1_2 = 0.0;

    double absValTwo = 0.0;

    double aV2_1 = 0.0;
    double aV2_2 = 0.0;

    int wB1 = 0;
    int wB2 = 0;

	int cntOne = 0;
	int cntTwo = 0;

    int track = 0;


    while(getline(f, line)) {
		size_t posr;
        track++;

		if((posr = line.find("#")) == 0){
			//std::cout << "ignoring line" << std::endl;
			continue;
		}

		if(line[0] == '\n'){
			//std::cout << "ignoring line" << std::endl;
			continue;
		}

        std::string s = line;
		std::string delimiter = ";";

		int cat = 0;
		size_t pos = 0;
		std::string token;


		while ((pos = s.find(delimiter)) != std::string::npos) {

			token = s.substr(0, pos);

			if(cat==0){
				try{
					linenumber = std::stoi(token);
				}catch(...){
					errors++;
                    //std::cout << "linenumber" << std::endl;
                    //std::cout << track << std::endl;
					break;
				}
				cat=1;
				s.erase(0, pos + delimiter.length());
			}

			else if(cat==1){
				try{
					category = std::stoi(token);
				}catch(...){
					errors++;
                    //std::cout << "cat" << std::endl;
                    // std::cout << track << std::endl;
					break;
				}

				s.erase(0, pos + delimiter.length());

				try{
					value = std::stod(s);

					//Checking if value is a proper double format
					if(!(value < 0.0 || value >= 0.0)){
						errors++;
						break;
					}


                    if(value<=0){
                        //std::cout << "errsth0" << std::endl;
                        break;
                    }

				}catch(...){
					errors++;
                    //std::cout << "val" << std::endl;
                    //std::cout << track << std::endl;
					break;
				}

				if(category==1){
					cntOne++;
					absValOne+= log(value);



				}
				else if(category==2){
					cntTwo++;
					absValTwo += log(value);

				}
                else break;
				cat=0;


			}

		}

		/*if(linenumber%100000==0){
					std::cout << linenumber << std::endl;
					std::cout << "Category One" << std::endl;
					std::cout << aV1_1 << std::endl;
					std::cout << aV1_2 << std::endl;
					std::cout << cntOne << std::endl;
					std::cout << absValOne/cntOne << std::endl;
					std::cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;

					std::cout << "Category Two" << std::endl;
					std::cout << aV2_1 << std::endl;
					std::cout << aV2_2 << std::endl;
					std::cout << cntTwo << std::endl;
					std::cout << absValTwo/cntTwo << std::endl;
					std::cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;

					std::cout << "Errors occured" << std::endl;
					std::cout << errors << std::endl;
					std::cout << "\n" << std::endl;
			}*/


    }

	f.close();
    std::cout << "Results:" << std::endl;
	std::cout << "Category One" << std::endl;
	std::cout << "Valid lines: ";
	std::cout << cntOne << std::endl;
	std::cout << "Geo Mean: ";
	//std::cout << exp(aV1_1/cntOne + aV1_2/cntOne) << std::endl;
	std::cout << exp(absValOne/cntOne) << std::endl;
	std::cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;

	std::cout << "Category Two" << std::endl;
	std::cout << "Valid lines: ";
	std::cout << cntTwo << std::endl;
	std::cout << "Geo Mean: ";
	//std::cout << exp(aV2_1/cntOne + aV2_2/cntOne) << std::endl;
	std::cout << exp(absValTwo/cntTwo) << std::endl;
	std::cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;

	std::cout << "Errors occured" << std::endl;
	std::cout << errors << std::endl;


    return 0;
}




