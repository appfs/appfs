/*
 * ParserErrorHandler.cpp
 *
 *  Created on: 01.05.2017
 *      Author: alesan
 */

#include "ParserErrorHandler.h"

ParserErrorHandler::ParserErrorHandler() : HandlerBase::HandlerBase() {
	errors = vector<string>();
}
void ParserErrorHandler::error(const SAXParseException& exc) {
	HandlerBase::error(exc);
	addException(exc, "Exception");
}

void ParserErrorHandler::fatalError(const SAXParseException& exc) {
	HandlerBase::fatalError(exc);
	addException(exc, "Fatal Exception");
}

void ParserErrorHandler::addException(const SAXParseException& exc, string exceptionType) {
	std::stringstream ss;
	char* message = XMLString::transcode(exc.getMessage());
	ss << exceptionType << " at line " << exc.getLineNumber() << " column " << exc.getColumnNumber() << ":" << endl << message << endl;
	XMLString::release(&message);
	errors.push_back(ss.str());
}

string ParserErrorHandler::getExceptionsAsString(){
	std::stringstream ss;
	for(unsigned int i=0; i<errors.size(); i++){
		ss << errors.at(i);
		ss << endl;
	}
	return ss.str();
}

