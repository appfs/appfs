/**
 * Exercise 2 : Read in a xml-file, validate via sheme and generate a csv with the values.
 *
 * @author FirstSanny
 */

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <vector>

#include "MeasurePointFromDocumentReader.h"
#include "XMLParser.h"
#include "MeasurePoint.h"

namespace {

	const char* OUTPUT_CSV = "output.csv";
	const char* SEPERATOR = "; ";

	const char* FILEPATH_MEASURED_XSD = "/home/alesan/git/appfs/Sanny/ex2/src/recources/EX2/measured-1-1-0.xsd";
	const char* FILEPATH_FRAMEWORK_XSD = "/home/alesan/git/appfs/Sanny/ex2/src/recources/EX2/Framework-1-1-0.xsd";
	const char* FILEPATH_PHYSICS_XSD = "/home/alesan/git/appfs/Sanny/ex2/src/recources/EX2/PhysicalValues-1-1-0.xsd";
	const char* FILEPATH_TOPO_XSD = "/home/alesan/git/appfs/Sanny/ex2/src/recources/EX2/Topology-1-1-0.xsd";

}

using namespace std;

void inline printException(const XMLException& toCatch) {
	char* message = XMLString::transcode(toCatch.getMessage());
	cerr << "Exception:" << endl << message << endl;
	XMLString::release(&message);
}

bool initGrammarIsSuccessfull(XMLParser* parser, const char* path) {
	if (parser->loadGrammar(path,
			Grammar::GrammarType::SchemaGrammarType, true) == nullptr) {
		cerr << "ERROR : Couldn't initialize XSD-Shema for file "
				<< path << endl;
		return 0;
	}
	return 1;
}

bool initGrammarAndCheckIfSuccessfull(XMLParser* parser) {
	if(!initGrammarIsSuccessfull(parser, FILEPATH_MEASURED_XSD)){
		return 0;
	}
	if(!initGrammarIsSuccessfull(parser, FILEPATH_PHYSICS_XSD)){
		return 0;
	}
	if(!initGrammarIsSuccessfull(parser, FILEPATH_FRAMEWORK_XSD)){
		return 0;
	}
	if(!initGrammarIsSuccessfull(parser, FILEPATH_TOPO_XSD)){
		return 0;
	}
	return 1;
}

int main(int argn, char *argv[]) {

	if (argn <= 1) {
		cerr << "ERROR : There was no filename" << endl;
		return 1;
	}

	try {
	    XMLPlatformUtils::Initialize();
	}catch (const XMLException& toCatch) {
		cerr << "ERROR : Couldn't initialize XML-Parser."<<endl;
		printException(toCatch);
		return 1;
	}

	XMLParser* parser = new XMLParser();

	bool success = initGrammarAndCheckIfSuccessfull(parser);

	if(!success){
		return 1;
	}

	try {
		parser->parse(argv[1]);

		if(parser->getErrorCount() != 0){
		  cerr << "ERROR : File doesn't match sheme." << endl;
		  cerr << "There were " << parser->getErrorCount() << " errors." << endl;
		  cerr << "Exceptions: "<< endl;
		  cerr << parser->getErrors();
//		  return 1;
		}

		MeasurePointFromDocumentReader* toFile = new MeasurePointFromDocumentReader(parser->getDocument());
		toFile->writeDocumentToFile();
		ofstream outputStream;
		outputStream.open(OUTPUT_CSV, ios::out);
		vector<MeasurePoint> measurePoints = toFile->getMeasurePoints();
		for(MeasurePoint mp : measurePoints){
			outputStream << mp.getDate() << SEPERATOR << setw(2) << mp.getHour() << SEPERATOR << mp.getPower() << endl;
		}
		delete toFile;

	}

	catch (const XMLException& toCatch) {
		cerr << "ERROR" <<endl;
		printException(toCatch);
		return 1;
	}

	catch (const SAXParseException& toCatch) {
		cerr << "ERROR : Exception while Parsing."<<endl;
		char* message = XMLString::transcode(toCatch.getMessage());
		cerr << "Exception:" << endl << message << endl;
		XMLString::release(&message);
	    return 1;
	}

	delete parser;

	XMLPlatformUtils::Terminate();

	return 0;
}
