/*
 * DocumentToFileWriter.h
 *
 *  Created on: 01.05.2017
 *      Author: alesan
 */

#ifndef C_DOCUMENTTOFILEWRITER_H_
#define C_DOCUMENTTOFILEWRITER_H_

#include <xercesc/util/XMLString.hpp>
#include <xercesc/dom/DOM.hpp>

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

using namespace xercesc;

class DocumentToFileWriter {
private:
	DOMDocument* document;
	std::istringstream strin;
	void writeMeasuredToFile(DOMNode* measuredNode, std::ofstream& outputFile);
	void writeBoundaryToFile(DOMNode* boundaryNode, std::string date, int dayLength, int startHour, std::ofstream& outputFile);
	void writeTimeToFile(DOMNode* timeNode, std::string date, int dayLength, int startHour, std::ofstream& outputFile);
	void writeAmountOfPowerToFile(DOMNode* amountOfPowerNode, std::string date, int dayLength, int startHour, std::ofstream& outputFile);
	void writeGasDayToFile(DOMNode* gasDayNode, std::ofstream& outputFile);
	std::string getNodeName(DOMNode* node);
	std::string getNodeValueAsString(DOMNode* node);
	int getNodeValueAsInt(DOMNode* node);
	double getNodeValueAsDouble(DOMNode* node);
public:
	void writeDocumentToFile();
	DocumentToFileWriter(DOMDocument* document);
	virtual ~DocumentToFileWriter();
};

#endif /* C_DOCUMENTTOFILEWRITER_H_ */
