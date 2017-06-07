/*
 * ParserErrorHandler.cpp
 *
 *  Created on: 01.05.2017
 *      Author: alesan
 */

#include "ParserErrorHandler.h"

ParserErrorHandler::ParserErrorHandler() : HandlerBase::HandlerBase() {
	errors = vector<string>();
	fatalErrors = vector<string>();
	warnings = vector<string>();
}

void ParserErrorHandler::warning(const SAXParseException& exc) {
	HandlerBase::warning(exc);
	addException(warnings, exc);
}

void ParserErrorHandler::error(const SAXParseException& exc) {
	HandlerBase::error(exc);
	addException(errors, exc);
}

void ParserErrorHandler::fatalError(const SAXParseException& exc) {
	HandlerBase::fatalError(exc);
	addException(fatalErrors, exc);
}

void ParserErrorHandler::addException(vector<string> &list, const SAXParseException& exc) {
	std::stringstream ss;
	char* message = XMLString::transcode(exc.getMessage());
	ss << "Exception at line " << exc.getLineNumber() << " column " << exc.getColumnNumber() << ":" << endl << message << endl;
	XMLString::release(&message);
	list.push_back(ss.str());
}


string ParserErrorHandler::getExceptionsAsString(){
	std::stringstream ss;
	ss << warnings.size() <<" Warnings:" << endl;
	addStrings(ss, warnings);
	ss << endl << errors.size() <<  " Errors:" << endl;
	addStrings(ss, errors);
	ss << endl << fatalErrors.size() << " Fatal Errors:" << endl;
	addStrings(ss, fatalErrors);
	return ss.str();
}

void ParserErrorHandler::addStrings(std::stringstream& ss, vector<string> &strings) {
	for (unsigned int i = 0; i < strings.size(); i++) {
		ss << strings.at(i);
	}
}

