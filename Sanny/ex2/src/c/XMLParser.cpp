/*
 * XMLParser.cpp
 *
 *  Created on: 01.05.2017
 *      Author: alesan
 */

#include "XMLParser.h"

XMLParser::XMLParser() : XercesDOMParser(){
	errorHandler = new ParserErrorHandler();

	XercesDOMParser::setValidationScheme(XercesDOMParser::Val_Always);
	XercesDOMParser::setHandleMultipleImports(true);
	XercesDOMParser::setDoSchema(true);
	XercesDOMParser::setDoNamespaces(true);
	XercesDOMParser::setErrorHandler(errorHandler);
	XercesDOMParser::setValidationSchemaFullChecking(true);
	XercesDOMParser::setLoadSchema(true);
}

XMLParser::~XMLParser(){
	delete errorHandler;
}

Grammar* XMLParser::loadGrammar(const char* const systemId,
        const Grammar::GrammarType grammarType,
        const bool toCache){
	return XercesDOMParser::loadGrammar(systemId, grammarType, toCache);
}

string XMLParser::getErrors(){
	return errorHandler->getExceptionsAsString();
}

