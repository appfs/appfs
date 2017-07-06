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

/** Parser for an XML file, which parses into a DOM */
class XMLParser : public XercesDOMParser{
private:
	ParserErrorHandler* errorHandler;
public:
	/** Constructor */
	XMLParser();
	/** Deconstructor */
	virtual ~XMLParser();
	/** loads given Grammar */
	Grammar* loadGrammar(const char* const systemId,
	                         const Grammar::GrammarType grammarType,
	                         const bool toCache = false);
	/** Delegate to gget all the errors from ErrorHandler */
	string getErrors();
};

#endif /* C_XMLPARSER_H_ */
