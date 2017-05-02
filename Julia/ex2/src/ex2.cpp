#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include "tinyxml2.h"
#include "GasNode.h"
#include <xercesc/util/PlatformUtils.hpp>

using namespace tinyxml2;


XMLError checkResult(XMLError result){
	if (result != XML_SUCCESS) {
		std::cout << "Error while opening file: " << result << std::endl;
	}
	return result;
}

void checkAttribute(const XMLAttribute* attribute){
	if (attribute == 0) {
		std::cout << "Error: Attribute " << attribute << " not found." << std::endl;
	}
}




int main(int argc, char* argv[]){
	try {
		xercesc::XMLPlatformUtils::Initialize();
	}
	catch (const xercesc::XMLException& toCatch) {
		return 1;
	}


	XMLDocument xmlDoc;

	const XMLAttribute *attribute;
	XMLError result;
	if (argc <= 1){
		std::cout << "No path found. Open default file " << std::endl;
		result = xmlDoc.LoadFile("example.measured-1-1-0.xml");
	} else {
		result = xmlDoc.LoadFile(argv[1]);
	}

	checkResult(result);


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
	std::string date = dateOut;

	int startingHour;
	attribute = gasElement -> FindAttribute("gasDayStartHourInUTC");
	checkAttribute(attribute);
	startingHour = attribute -> IntValue();


	XMLElement * boundaryElement = gasElement -> FirstChildElement("boundaryNode");
	XMLElement * timeElement;
	XMLElement * powerElement;
	std::vector<GasNode> dataNodes;
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



	xercesc::XMLPlatformUtils::Terminate();


	std::fstream output;
	output.open("measured.csv", std::ios::out);


	for (int i = 0; i < dataNodes.size(); ++i) {
		output << date << "; " << dataNodes.at(i).getTime() + startingHour << "; " << dataNodes.at(i).getPower() << std::endl;
	}

	output.close();

	return 0;

}
