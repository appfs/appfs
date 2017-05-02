/*
 * DocumentToFileWriter.cpp
 *
 *  Created on: 01.05.2017
 *      Author: alesan
 */

#include "DocumentToFileWriter.h"

namespace{
	const char* OUTPUT_CSV = "output.csv";
	const char* SEPERATOR = "; ";

	const char* GASDAY_NODE = "gasDay";
	const char* BOUNDARY_NODE = "boundaryNode";
	const char* TIME_NODE = "time";
	const char* AMOUNTOFPOWER_NODE = "amountOfPower";

}

	XMLCh* NODE_gasDay;

	XMLCh* ATTR_date;
	XMLCh* ATTR_gasDayStartHourInUTC;
	XMLCh* ATTR_gasDayLengthInHours;
	XMLCh* ATTR_hour;
	XMLCh* ATTR_value;


DocumentToFileWriter::DocumentToFileWriter(DOMDocument* document){
	XMLPlatformUtils::Initialize();

	initNodes();
	initAttributes();

	this->document = document;
}

void DocumentToFileWriter::initNodes() {
	NODE_gasDay = XMLString::transcode(GASDAY_NODE);
}

void DocumentToFileWriter::initAttributes() {
	ATTR_date = XMLString::transcode("date");
	ATTR_gasDayStartHourInUTC = XMLString::transcode("gasDayStartHourInUTC");
	ATTR_gasDayLengthInHours = XMLString::transcode("gasDayLengthInHours");
	ATTR_hour = XMLString::transcode("hour");
	ATTR_value = XMLString::transcode("value");
}

void DocumentToFileWriter::writeDocumentToFile() {
	ofstream outputFile;
	outputFile.open(OUTPUT_CSV);

	DOMNodeList* nodeList = document->getElementsByTagName(NODE_gasDay);
	for(unsigned int i = 0; i < nodeList->getLength(); i++){
		writeGasDayToFile(nodeList->item(i), outputFile);
	}
}


void DocumentToFileWriter::writeGasDayToFile(DOMNode* gasDayNode, ofstream& outputFile) {
	std::string date = "";
	int daylength = 0;
	int startHour = 0;
	DOMNamedNodeMap* attributeMap = gasDayNode->getAttributes();
	date = getNodeValueAsString(attributeMap->getNamedItem(ATTR_date));
	daylength = getNodeValueAsInt(attributeMap->getNamedItem(ATTR_gasDayLengthInHours));
	startHour = getNodeValueAsInt(attributeMap->getNamedItem(ATTR_gasDayStartHourInUTC));
	DOMNodeList* nodeList = gasDayNode->getChildNodes();
	for(unsigned int i = 0; i < nodeList->getLength(); i++){
		DOMNode* node = nodeList->item(i);
		if(this->getNodeName(node)==BOUNDARY_NODE){
			this->writeBoundaryToFile(node, date, daylength, startHour, outputFile);
		}
	}
}

void DocumentToFileWriter::writeBoundaryToFile(DOMNode* boundaryNode, std::string date, int dayLength, int startHour, std::ofstream& outputFile) {
	DOMNodeList* nodeList = boundaryNode->getChildNodes();
	for(unsigned int i = 0; i < nodeList->getLength(); i++){
		DOMNode* node = nodeList->item(i);
		if(this->getNodeName(node)==TIME_NODE){
			this->writeTimeToFile(node, date, dayLength, startHour, outputFile);
		}
	}
}

void DocumentToFileWriter::writeTimeToFile(DOMNode* timeNode, std::string date, int dayLength, int startHour, std::ofstream& outputFile) {
	DOMNamedNodeMap* attributeMap = timeNode->getAttributes();
	int hour = getNodeValueAsInt(attributeMap->getNamedItem(ATTR_hour));
	hour = (startHour+hour)%dayLength;

	DOMNodeList* nodeList = timeNode->getChildNodes();
	for(unsigned int i = 0; i < nodeList->getLength(); i++){
		DOMNode* node = nodeList->item(i);
		if(this->getNodeName(node)==AMOUNTOFPOWER_NODE){
			writeAmountOfPowerToFile(node, date, dayLength, hour, outputFile);
		}
	}


}
void DocumentToFileWriter::writeAmountOfPowerToFile(DOMNode* powerNode, const std::string date, int dayLength, int hour, std::ofstream& outputFile) {
	DOMNamedNodeMap* attributeMap = powerNode->getAttributes();
	double power = getNodeValueAsDouble(attributeMap->getNamedItem(ATTR_value));
	outputFile << date << SEPERATOR <<std::setw(2) << hour << SEPERATOR << power << std::endl;
}

std::string DocumentToFileWriter::getNodeName(DOMNode* node){
	std::stringstream ss;
	char* name = XMLString::transcode(node->getNodeName());
	ss << name;
	XMLString::release(&name);
	return ss.str();
}

std::string DocumentToFileWriter::getNodeValueAsString(DOMNode* node){
	std::stringstream ss;
	char* value = XMLString::transcode(node->getNodeValue());
	ss << value;
	XMLString::release(&value);
	return ss.str();
}

int DocumentToFileWriter::getNodeValueAsInt(DOMNode* node){
	char* value = XMLString::transcode(node->getNodeValue());
	strin.str(value);
	int valueInt;
	strin >> valueInt;
	XMLString::release(&value);
	strin.clear();
	return valueInt;
}

double DocumentToFileWriter::getNodeValueAsDouble(DOMNode* node){
	char* value = XMLString::transcode(node->getNodeValue());
	strin.str(value);
	double valueDouble;
	strin >> valueDouble;
	XMLString::release(&value);
	strin.clear();
	return valueDouble;
}

DocumentToFileWriter::~DocumentToFileWriter(){
    XMLString::release( &NODE_gasDay );
	XMLString::release( &ATTR_date );
	XMLString::release( &ATTR_gasDayLengthInHours );
	XMLString::release( &ATTR_gasDayStartHourInUTC);
	XMLString::release( &ATTR_hour);
	XMLString::release( &ATTR_date);
	XMLPlatformUtils::Terminate();
}


