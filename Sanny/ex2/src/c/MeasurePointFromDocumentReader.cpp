/*
 * DocumentToFileWriter.cpp
 *
 *  Created on: 01.05.2017
 *      Author: alesan
 */

#include "MeasurePointFromDocumentReader.h"

namespace{

	const char* GASDAY_NODE = "gasDay";
	const char* BOUNDARY_NODE = "boundaryNode";
	const char* TIME_NODE = "time";
	const char* AMOUNTOFPOWER_NODE = "amountOfPower";

}


MeasurePointFromDocumentReader::MeasurePointFromDocumentReader(DOMDocument* document){
	XMLPlatformUtils::Initialize();

	initNodes();
	initAttributes();

	this->document = document;
}

void MeasurePointFromDocumentReader::initNodes() {
	NODE_gasDay = XMLString::transcode(GASDAY_NODE);
}

void MeasurePointFromDocumentReader::initAttributes() {
	ATTR_date = XMLString::transcode("date");
	ATTR_gasDayStartHourInUTC = XMLString::transcode("gasDayStartHourInUTC");
	ATTR_gasDayLengthInHours = XMLString::transcode("gasDayLengthInHours");
	ATTR_hour = XMLString::transcode("hour");
	ATTR_value = XMLString::transcode("value");
}

void MeasurePointFromDocumentReader::readMeasurepoints() {
	measurePoints.clear();
	DOMNodeList* nodeList = document->getElementsByTagName(NODE_gasDay);
	for(unsigned int i = 0; i < nodeList->getLength(); i++){
		readGasDayNode(nodeList->item(i));
	}
}


void MeasurePointFromDocumentReader::readGasDayNode(DOMNode* gasDayNode) {
	string date = "";
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
			this->readBoundaryNode(node, date, daylength, startHour);
		}
	}
}

void MeasurePointFromDocumentReader::readBoundaryNode(DOMNode* boundaryNode, string date, int dayLength, int startHour) {
	DOMNodeList* nodeList = boundaryNode->getChildNodes();
	for(unsigned int i = 0; i < nodeList->getLength(); i++){
		DOMNode* node = nodeList->item(i);
		if(this->getNodeName(node)==TIME_NODE){
			this->readTimeNode(node, date, dayLength, startHour);
		}
	}
}

void MeasurePointFromDocumentReader::readTimeNode(DOMNode* timeNode, string date, int dayLength, int startHour) {
	DOMNamedNodeMap* attributeMap = timeNode->getAttributes();
	int hour = getNodeValueAsInt(attributeMap->getNamedItem(ATTR_hour));
	hour = (startHour+hour)%dayLength;

	DOMNodeList* nodeList = timeNode->getChildNodes();
	for(unsigned int i = 0; i < nodeList->getLength(); i++){
		DOMNode* node = nodeList->item(i);
		if(this->getNodeName(node)==AMOUNTOFPOWER_NODE){
			readAmountOfPowerNode(node, date, dayLength, hour);
		}
	}


}
void MeasurePointFromDocumentReader::readAmountOfPowerNode(DOMNode* powerNode, const string date, int dayLength, int hour) {
	DOMNamedNodeMap* attributeMap = powerNode->getAttributes();
	double power = getNodeValueAsDouble(attributeMap->getNamedItem(ATTR_value));
	MeasurePoint* point = new MeasurePoint(power,hour,date);
	measurePoints.push_back(point);
}

string MeasurePointFromDocumentReader::getNodeName(DOMNode* node){
	stringstream ss;
	char* name = XMLString::transcode(node->getNodeName());
	ss << name;
	XMLString::release(&name);
	return ss.str();
}

string MeasurePointFromDocumentReader::getNodeValueAsString(DOMNode* node){
	stringstream ss;
	char* value = XMLString::transcode(node->getNodeValue());
	ss << value;
	XMLString::release(&value);
	return ss.str();
}

int MeasurePointFromDocumentReader::getNodeValueAsInt(DOMNode* node){
	char* value = XMLString::transcode(node->getNodeValue());
	strin.str(value);
	int valueInt;
	strin >> valueInt;
	XMLString::release(&value);
	strin.clear();
	return valueInt;
}

double MeasurePointFromDocumentReader::getNodeValueAsDouble(DOMNode* node){
	char* value = XMLString::transcode(node->getNodeValue());
	strin.str(value);
	double valueDouble;
	strin >> valueDouble;
	XMLString::release(&value);
	strin.clear();
	return valueDouble;
}

vector<MeasurePoint*> MeasurePointFromDocumentReader::getMeasurePoints(){
	return measurePoints;
}

MeasurePointFromDocumentReader::~MeasurePointFromDocumentReader(){
    XMLString::release( &NODE_gasDay );
	XMLString::release( &ATTR_date );
	XMLString::release( &ATTR_gasDayLengthInHours );
	XMLString::release( &ATTR_gasDayStartHourInUTC);
	XMLString::release( &ATTR_hour);
	XMLString::release( &ATTR_date);
	XMLPlatformUtils::Terminate();
}


