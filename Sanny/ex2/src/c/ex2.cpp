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

	const char* SEPERATOR = "; ";


	const char* URL_MEASURED_XSD = "http://gaslab.zib.de/kwpt/measured";
	const char* FILE_MEASURED_XSD = "measured-1-1-0.xsd";
//	const char* FILE_FRAMEWORK_XSD = "Framework-1-1-0.xsd";
//	const char* FILE_PHYSICS_XSD = "PhysicalValues-1-1-0.xsd";
//	const char* FILE_TOPO_XSD = "Topology-1-1-0.xsd";
//	const char* FILE_PROG_XSD = "prognosis-1-1-0.xsd";

}

using namespace std;

void inline printException(const XMLException& toCatch) {
	char* message = XMLString::transcode(toCatch.getMessage());
	cerr << "Exception:" << endl << message << endl;
	XMLString::release(&message);
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
		return 0;
	}

	string filename = argv[1];

	XMLParser* parser = new XMLParser();

	try {
		string shemaLoc = URL_MEASURED_XSD;
		shemaLoc = shemaLoc + " " + FILE_MEASURED_XSD;
		parser->setExternalSchemaLocation(shemaLoc.c_str());
		parser->parse(argv[1]);

		if(parser->getErrorCount() != 0){
		  cerr << "ERROR : File doesn't match sheme." << endl;
		  cerr << "There were " << parser->getErrorCount() << " errors." << endl;
		  cerr << "Exceptions: "<< endl;
		  cerr << parser->getErrors();
		  return 0;
		}

		MeasurePointFromDocumentReader* measurePointsReader = new MeasurePointFromDocumentReader(parser->getDocument());
		measurePointsReader->writeDocumentToFile();
		ofstream outputStream;

		string output=filename.replace(filename.end()-3, filename.end(),"csv");
		outputStream.open(output.c_str(), ios::out);

		vector<MeasurePoint> measurePoints = measurePointsReader->getMeasurePoints();
		for(MeasurePoint mp : measurePoints){
			outputStream << mp.getDate() << SEPERATOR << setw(2) << mp.getHour() << SEPERATOR << mp.getPower() << endl;
		}

		delete measurePointsReader;

	}

	catch (const XMLException& toCatch) {
		cerr << "ERROR" <<endl;
		printException(toCatch);
		return 0;
	}

	catch (const SAXParseException& toCatch) {
		cerr << "ERROR : Exception while Parsing."<<endl;
		char* message = XMLString::transcode(toCatch.getMessage());
		cerr << "Exception:" << endl << message << endl;
		XMLString::release(&message);
	    return 0;
	}

	delete parser;

	XMLPlatformUtils::Terminate();

	return 1;
}
