#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/framework/LocalFileInputSource.hpp>
#include <xercesc/sax/ErrorHandler.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/sax/SAXParseException.hpp>
#include <xercesc/validators/common/Grammar.hpp>
#include "tinyxml2.h"
#include "GasNode.h"
#include "DocumentErrorHandler.h"

using namespace tinyxml2;
using namespace std;
using namespace xercesc;

const char* FILE_PATH = "measured.xml";
const char* MEASURED_XSD = "http://gaslab.zib.de/kwpt/measured measured-1-1-0.xsd";


XMLError checkResult(XMLError result){
	if (result != XML_SUCCESS) {
		cerr << "Error while opening file: " << result << endl;
	}
	return result;
}

void checkAttribute(const XMLAttribute* attribute){
	if (attribute == 0) {
		cerr << "Error: Attribute " << attribute << " not found." << endl;
	}
}

bool documentIsValid(const char* path){
	XercesDOMParser domParser;

	DocumentErrorHandler errorHandler;
	domParser.setDoNamespaces(true);
	domParser.setExternalSchemaLocation(MEASURED_XSD);
	domParser.setErrorHandler(&errorHandler);
	domParser.setValidationScheme(XercesDOMParser::Val_Auto);
	domParser.setDoSchema(true);
	domParser.setValidationConstraintFatal(true);

	try {
		domParser.parse(FILE_PATH);
	} catch (SAXParseException&) {
		cerr << "Error while parsing!" << endl;
		return false;
	}
	if (domParser.getErrorCount() == 0) {
	     cout << "XML file validated against the schema successfully" << endl;
	     return true;
	}else {
	     cerr << "XML file doesn't conform to the schema" << endl;
	     return true;
	}
}


int main(int argc, char* argv[]){
	try {
		XMLPlatformUtils::Initialize();
	}catch (const XMLException& toCatch) {
		return 0;
	}

	XMLDocument xmlDoc;

	XMLError result;
	if (argc <= 1){
		cerr << "No path found. Open default file " << endl;
		result = xmlDoc.LoadFile(FILE_PATH);
	} else {
		result = xmlDoc.LoadFile(argv[1]);
		FILE_PATH = argv[1];
	}

	checkResult(result);


	if (!documentIsValid(FILE_PATH)) {
		cerr << "Document is not valid!" << endl;
		XMLPlatformUtils::Terminate();
		return 0;
	}

	XMLElement * rootElement = xmlDoc.FirstChildElement();
	if (rootElement == nullptr){
		return XML_ERROR_PARSING_ELEMENT;
	}

	XMLElement * gasElement = rootElement -> FirstChildElement("gasDay");
	if (gasElement == nullptr){
			return XML_ERROR_PARSING_ELEMENT;
	}

	//Reading the date
	const char* dateOut = nullptr;
	dateOut = gasElement -> Attribute("date");
	if (nullptr == dateOut) {
		return XML_ERROR_PARSING_ATTRIBUTE;
	}
	string date = dateOut;

	const XMLAttribute *attribute;

	//Reading at which hour the gas day starts
	int startingHour;
	attribute = gasElement -> FindAttribute("gasDayStartHourInUTC");
	checkAttribute(attribute);
	startingHour = attribute -> IntValue();


	//Reading data from the nodes
	XMLElement * boundaryElement = gasElement -> FirstChildElement("boundaryNode");
	XMLElement * timeElement;
	XMLElement * powerElement;
	vector<GasNode> dataNodes;
	GasNode dataNode;
	int time;
	int value;

	while(boundaryElement != nullptr) {
		timeElement  = boundaryElement -> FirstChildElement("time");
		while (timeElement != nullptr){
			attribute = timeElement -> FindAttribute("hour");
			checkAttribute(attribute);
			time = attribute -> IntValue();

			powerElement = timeElement -> FirstChildElement("amountOfPower");
			attribute = powerElement -> FindAttribute("value");
			checkAttribute(attribute);
			value = attribute -> IntValue();

			dataNode.setTime(time);
			dataNode.setPower(value);

			dataNodes.push_back(dataNode);

			timeElement = timeElement -> NextSiblingElement("time");
		}
		boundaryElement = boundaryElement -> NextSiblingElement("boundaryNode");
	}

	//Writing the new .csv file
	fstream output;
	output.open("measured.csv", ios::out);

	for (unsigned int i = 0; i < dataNodes.size(); ++i) {
		output << date << "; " << dataNodes.at(i).getTime() + startingHour << "; " << dataNodes.at(i).getPower() << std::endl;
	}

	output.close();



    XMLPlatformUtils::Terminate();

	return 1;

}
