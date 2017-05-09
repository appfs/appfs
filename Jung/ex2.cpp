// reads in measured-1.0.0.2017-02-03.b0050c5c8deb1db59c7b2644414b079d.xml and creates an csv file\
// I used RapidXml as xml-Parser (http://rapidxml.sourceforge.net)




#include <iostream>
#include <string>
#include "rapidxml.hpp"
#include <fstream>
#include <sstream>

using namespace std;

int main(int argc, char* argv[]) {


	string input;
	cout << "enter the name of the csv file you want to create:" << endl;
	cin >> input;
		
		
		rapidxml::xml_document<> doc;
		ifstream file("measured-1.0.0.2017-02-03.b0050c5c8deb1db59c7b2644414b079d.xml");
	
		//checks if the file can be opened
		if (!file.is_open()) {
			cerr << "couldnt open/find the file " << endl;
			return 0;
		}


		//loading XML-File
		stringstream buffer;
		buffer << file.rdbuf();
		file.close();
		string content(buffer.str());
		doc.parse<0>(&content[0]);

		//creates output file
		ofstream outputFile;
		outputFile.open(input+".csv");


		//start reading////////////////


		// get the root( measured node)
		rapidxml::xml_node<> *pRoot = doc.first_node();


		string s = pRoot->name();
		//
		if (s.compare("measured")!=0) {
			cerr << "Your xml file doesnt fit the schema1";
			return 0;
		}

		// go through gasday nodes
		for (rapidxml::xml_node<> *gasNode = pRoot->first_node("gasDay"); gasNode; gasNode = gasNode->next_sibling()) {
			


			rapidxml::xml_attribute<> *pAttr = gasNode->first_attribute("date");

			string date = pAttr->value();

			double value[25] = { 0.0 };
			bool measured[25] = { false };

			//go through boundary nodes
			for (rapidxml::xml_node<> *bdNode = gasNode->first_node("boundaryNode"); bdNode; bdNode = bdNode->next_sibling()) {
				
				//go through hours
				for (rapidxml::xml_node<> *timeNode = bdNode->first_node("time"); timeNode; timeNode = timeNode->next_sibling()) {


					// consider only measurements on minute=0
					if (atoi((timeNode->first_attribute("minute"))->value()) == 0) {

						int k = atoi(timeNode->first_attribute("hour")->value());
					
						string AlsString(((timeNode->first_node("amountOfPower"))->first_attribute("value"))->value());
						stringstream Str;
						Str << AlsString;
						double d;
						Str >> d;
						value[k] = value[k]+d;
						measured[k] = true;
						
					}
				}
			}

			bool t = false;
			//write the measurements of the gasday to the file
			for (int i = 0; i < 25; i++) {
				if (measured[i]) {
					t = true;
					if (i > 9) {
						outputFile << date << "; " << i << "; " << value[i] << endl;
					}
					else {
						outputFile << date << "; 0" << i << "; " << value[i] << endl;
					}
				}
			}

			if (!t) {
				cerr<<"no valid measurements"<<endl;
			}

		}

	}

