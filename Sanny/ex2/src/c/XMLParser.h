/*
 * XMLParser.h
 *
 *  Created on: 01.05.2017
 *      Author: alesan
 */

#ifndef C_XMLPARSER_H_
#define C_XMLPARSER_H_

#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOM.hpp>
#include <fstream>
#include <string>

#include "ParserErrorHandler.h"

using namespace xercesc;

class XMLParser : public XercesDOMParser{
private:
	ParserErrorHandler* errorHandler;
public:
	XMLParser();
	virtual ~XMLParser();
	Grammar* loadGrammar(const char* const systemId,
	                         const Grammar::GrammarType grammarType,
	                         const bool toCache = false);
	string getErrors();
};

#endif /* C_XMLPARSER_H_ */
