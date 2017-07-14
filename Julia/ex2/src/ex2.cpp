/*
 * ex2.cpp
 *
 * 	\brief     Second exercixe
 *  \details   This class reads in a xml-file and validates it against a scheme. Also reads data from given file. Run program with ex2 filename.
 *  \pre       Be sure that the validation scheme is there
 *  \author    Julia Baumbach
 *  \date      01.05.2017
 */

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

/**
 * \param SOURCE_FILE_PATH default file path if none is hand over while starting the programm
 */
const char* SOURCE_FILE_PATH = "measured.xml";
/**
 * \param SCHEME_PATH path for validation scheme
 */
const char* SCHEME_PATH = "http://gaslab.zib.de/kwpt/measured measured-1-1-0.xsd";

/**
 * \fn void checkResult(XMLError result)
 * \brief Checks if a XMLError is a success or not.
 * If not, writes an error message at the console
 * \param result the XMLError to check
 */
void checkResult(XMLError result){
	if (result != XML_SUCCESS) {
		cerr << "Error while opening file: " << result << endl;
	}
}

/**
 * \fn void checkAttribute(const XMLAttribute* attribute)
 * \brief Checks if a given attribute is valid. If not,prints an error
 * to the console
 * \param attribute the given attribute to check
 */
void checkAttribute(const XMLAttribute* attribute){
	if (attribute == 0) {
		cerr << "Error: Attribute " << attribute << " not found." << endl;
	}
}

/**
 * \fn void validateDocument()
 * \brief Validates the given document against the validation scheme.
 * If it's not valid, prints the validation errors and warnings to the console
 */
void validateDocument(){
	XercesDOMParser domParser;

	DocumentErrorHandler errorHandler;
	domParser.setDoNamespaces(true);
	domParser.setExternalSchemaLocation(SCHEME_PATH);
	domParser.setErrorHandler(&errorHandler);
	domParser.setValidationScheme(XercesDOMParser::Val_Auto);
	domParser.setDoSchema(true);
	domParser.setValidationConstraintFatal(true);

	try {
		domParser.parse(SOURCE_FILE_PATH);
	} catch (SAXParseException&) {
		cerr << "Error while parsing!" << endl;
	}
	if (domParser.getErrorCount() == 0) {
	     cout << "XML file validated against the schema successfully" << endl;
	}else {
	     cerr << "XML file doesn't conform to the schema. " << domParser.getErrorCount() << " error(s) found." << endl;
	}
}


/**
 * Main function which reads in a document and validates it.
 * Reads also data from the given document.
 * Shows error message if the file could not be found
 */
int main(int argc, char* argv[]){
	try {
		XMLPlatformUtils::Initialize();
	}catch (const XMLException& toCatch) {
		return 1;
	}

	XMLDocument xmlDoc;

	XMLError result;
	if (argc <= 1){
		cerr << "No path found. Try to open default file " << endl;
	} else {
		SOURCE_FILE_PATH = argv[1];
	}
	result = xmlDoc.LoadFile(SOURCE_FILE_PATH);

	//Check result and validate document
	checkResult(result);
	validateDocument();

	//Read data from document
	XMLElement * rootElement = xmlDoc.FirstChildElement();
	if (rootElement == nullptr){
		return XML_ERROR_PARSING_ELEMENT;
	}

	XMLElement * gasElement = rootElement -> FirstChildElement("gasDay");
	if (gasElement == nullptr){
			return XML_ERROR_PARSING_ELEMENT;
	}

	const char* dateOut = nullptr;
	dateOut = gasElement -> Attribute("date");
	if (nullptr == dateOut) {
		return XML_ERROR_PARSING_ATTRIBUTE;
	}
	string date = dateOut;

	const XMLAttribute *attribute;

	int startingHour;
	attribute = gasElement -> FindAttribute("gasDayStartHourInUTC");
	checkAttribute(attribute);
	startingHour = attribute -> IntValue();


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
	string target_file_path;
	stringstream s(SOURCE_FILE_PATH);
	s >> target_file_path;
	size_t pos = target_file_path.find_first_of('.');

	target_file_path = target_file_path.substr(0, pos) + ".csv";

	fstream output;
	output.open(target_file_path, ios::out);

	for (unsigned int i = 0; i < dataNodes.size(); ++i) {
		output << date << "; " << dataNodes.at(i).getTime() + startingHour << "; " << dataNodes.at(i).getPower() << std::endl;
	}

	output.close();

    XMLPlatformUtils::Terminate();
	cout << "Finished writing " << target_file_path << endl;

	return 0;

}
