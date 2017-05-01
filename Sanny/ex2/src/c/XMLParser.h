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
#include <xercesc/sax/HandlerBase.hpp>
#include <fstream>
#include <string>

using namespace xercesc;

class XMLParser : public XercesDOMParser{
private:
	ErrorHandler* errorHandler;
public:
	XMLParser();
	virtual ~XMLParser();
	Grammar* loadGrammar(const char* const systemId,
	                         const Grammar::GrammarType grammarType,
	                         const bool toCache = false);
	int getErrorCount();
};

#endif /* C_XMLPARSER_H_ */
