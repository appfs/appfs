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

	const char* MEASURED_NODE = "measured";
	const char* GASDAY_NODE = "gasDay";
	const char* BOUNDARY_NODE = "boundaryNode";
	const char* TIME_NODE = "time";
	const char* AMOUNTOFPOWER_NODE = "amountOfPower";

	const char* DATE_ATTRIBUTE = "date";
	const char* STARTHOUR_ATTRIBUTE = "gasDayStartHourInUTC";
	const char* DAYLENGTH_ATTRIBUTE = "gasDayLengthInHours";
	const char* HOUR_ATTRIBUTE = "hour";
	const char* VALUE_ATTRIBUTE = "value";
}

DocumentToFileWriter::DocumentToFileWriter(DOMDocument* document){
	this->document = document;
}

void DocumentToFileWriter::writeDocumentToFile() {
	std::ofstream outputFile;
	outputFile.open(OUTPUT_CSV);
	DOMNodeList* nodeList = document->getChildNodes();
	for(unsigned int i = 0; i < nodeList->getLength(); i++){
		DOMNode* node = nodeList->item(i);
		if(this->getNodeName(node)==MEASURED_NODE){
			this->writeMeasuredToFile(node, outputFile);
		}
	}
}

void DocumentToFileWriter::writeMeasuredToFile(DOMNode* measuredNode, std::ofstream& outputFile) {
	DOMNodeList* nodeList = measuredNode->getChildNodes();
	for(unsigned int i = 0; i < nodeList->getLength(); i++){
		DOMNode* node = nodeList->item(i);
		if(this->getNodeName(node)==GASDAY_NODE){
			this->writeGasDayToFile(node, outputFile);
		}
	}
}

void DocumentToFileWriter::writeGasDayToFile(DOMNode* gasDayNode, std::ofstream& outputFile) {
	std::string date = "";
	int daylength = 0;
	int startHour = 0;
	DOMNamedNodeMap* attributeMap = gasDayNode->getAttributes();
	for(unsigned int i = 0; i < attributeMap->getLength(); i++){
		DOMNode* attribute = attributeMap->item(i);
		std::string attributeName = getNodeName(attribute);
		if(attributeName == DATE_ATTRIBUTE){
			date = getNodeValueAsString(attribute);
			continue;
		}
		if(attributeName == STARTHOUR_ATTRIBUTE){
			startHour = getNodeValueAsInt(attribute);
			continue;
		}
		if(attributeName == DAYLENGTH_ATTRIBUTE){
			daylength = getNodeValueAsInt(attribute);
			continue;
		}
	}
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
	int hour = startHour;
	for(unsigned int i = 0; i < attributeMap->getLength(); i++){
		DOMNode* attribute = attributeMap->item(i);
		std::string attributeName = getNodeName(attribute);
		if(attributeName == HOUR_ATTRIBUTE){
			hour = (hour + getNodeValueAsInt(attribute))%dayLength;
		}
	}
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
	for(unsigned int i = 0; i < attributeMap->getLength(); i++){
		DOMNode* attribute = attributeMap->item(i);
		std::string attributeName = getNodeName(attribute);
		if(attributeName == VALUE_ATTRIBUTE){
			outputFile << date << SEPERATOR <<std::setw(2) << hour << SEPERATOR << getNodeValueAsDouble(attribute) << std::endl;
		}
	}
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
	//nop
}


