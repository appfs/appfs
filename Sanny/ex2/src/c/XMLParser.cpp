/*
 * XMLParser.cpp
 *
 *  Created on: 01.05.2017
 *      Author: alesan
 */

#include "XMLParser.h"

XMLParser::XMLParser() : XercesDOMParser(){
	errorHandler = (ErrorHandler*) new HandlerBase();

	XercesDOMParser::setValidationScheme(XercesDOMParser::Val_Always);
	XercesDOMParser::setDoNamespaces(true);
	XercesDOMParser::setDoSchema(true);
	XercesDOMParser::setErrorHandler(errorHandler);
	XercesDOMParser::setValidationSchemaFullChecking(true);

}

XMLParser::~XMLParser(){
	delete errorHandler;
}

Grammar* XMLParser::loadGrammar(const char* const systemId,
        const Grammar::GrammarType grammarType,
        const bool toCache){
	return XercesDOMParser::loadGrammar(systemId, grammarType, toCache);
}

int XMLParser::getErrorCount(){
	return XercesDOMParser::getErrorCount();
}

