/**
 * @file ex2.cpp
 * @author Fabian Wegscheider
 * @date May 12, 2017
 */


#include <tinyxml/tinyxml.h>
#include <iostream>
#include <fstream>

using namespace std;


/**
 * recursively reads an XML file
 * of the type "measured"
 * @param node on first call root node
 * @param date date of node, but not used if node "gasDay"
 * @return a string representation of the results. returns empty string if something goes wrong
 */
string readRecursively(TiXmlNode *node, string date) {

	if (!node) return "";

	string output = "";

	if (node->Type() == TiXmlNode::TINYXML_ELEMENT) {			//if there are non-element nodes,
																//something must be wrong
		string value = node->Value();
		TiXmlElement *element = node->ToElement();

		if (value == "gasDay") {

			string currDate = element->Attribute("date");		//for gasDays we print the date once
			TiXmlNode *child = 0;								//and then pass it to its children
			output += "\n[Date: " + currDate + "]\n";
			while (child = node->IterateChildren(child)) {
				output += readRecursively(child, currDate);
			}

		} else if (value == "boundaryNode") {

			output += "\n" + (string) element->Attribute("id") + ": \n\n";
			TiXmlNode *child = 0;
			while (child = node->IterateChildren(child)) {
				output += readRecursively(child, date);
			}
		}

		else if (value == "time") {

			string hour = element->Attribute("hour");				//once we have found a time node
			if (hour.length() == 1) hour = "0" + hour;				//we can add the data to the string
			output += date + "; " + hour + "; ";
			output += (string) node->FirstChildElement("amountOfPower")->Attribute("value") + "\n";
		}

		else {
			cerr << "XML file does not have the right format" << endl;
			return "";
		}

	} else {
		cerr << "XML file does not have the right format" << endl;
		return "";
	}

	return output;
}


/**
 * main function which reads an xml file of the scheme "measured"
 * from the command line and prints the measured amounts of power
 * to the standard output.
 * @param numargs the number of inputs on the command line
 * @param args array of inputs on the command line
 * @return whether the file was read successfully
 */
int main(int numargs, char *args[]) {

	if (numargs != 2) {
		cout << "Usage: " << args[0] << " filename" << endl;
		return EXIT_FAILURE;
	}

	TiXmlDocument doc(args[1]);					//trying to read file
	bool loadOkay = doc.LoadFile();

	if (!loadOkay) {
		cout << "File could not be read." << endl;
		cout << doc.ErrorDesc();
		return EXIT_FAILURE;
	}


	TiXmlElement *root = doc.FirstChildElement();
	if ((string) root->Value() != "measured") {							//test if file fits the
		cerr << "XML file does not have the right format" << endl;		// measured schema
		return EXIT_FAILURE;
	}

	string output = "";
	TiXmlNode *child = 0;
	while (child = root->IterateChildren(child)) {						//for all child nodes (which
		string tmp = readRecursively(child, "");						//should be gasDay-nodes) a
		if (tmp.empty()) {												//call of the recursive function
			cerr << "XML file does not have the right format" << endl;	//is performed
			return EXIT_FAILURE;
		}
		output += tmp;
	}

	cout << output;
	return EXIT_SUCCESS;

}
