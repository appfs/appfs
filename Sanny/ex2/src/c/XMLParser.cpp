/*
 * XMLParser.cpp
 *
 *  Created on: 01.05.2017
 *      Author: alesan
 */

#include "XMLParser.h"

XMLParser::XMLParser() : XercesDOMParser(){
	errorHandler = (ErrorHandler*) new HandlerBase();

	AbstractDOMParser::setValidationScheme(XercesDOMParser::Val_Always);
	AbstractDOMParser::setDoNamespaces(true);
	AbstractDOMParser::setDoSchema(true);
	XercesDOMParser::setErrorHandler(errorHandler);
	AbstractDOMParser::setValidationSchemaFullChecking(true);

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
	return AbstractDOMParser::getErrorCount();
}

