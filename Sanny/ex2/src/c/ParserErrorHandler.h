/*
 * ParserErrorHandler.h
 *
 *  Created on: 01.05.2017
 *      Author: alesan
 */

#ifndef C_PARSERERRORHANDLER_H_
#define C_PARSERERRORHANDLER_H_

#include <xercesc/sax/HandlerBase.hpp>
#include <string>
#include <vector>
#include <sstream>
#include <xercesc/util/XMLString.hpp>

using namespace xercesc;
using namespace std;

class ParserErrorHandler : public HandlerBase{
private:
	vector<string> errors;
	vector<string> fatalErrors;
	vector<string> warnings;
	void addException(vector<string> &list, const xercesc_3_1::SAXParseException& exc);
	void addStrings(std::stringstream& ss, vector<string> &strings);

public:
	ParserErrorHandler();
	void error(const SAXParseException& exc);
	void fatalError(const SAXParseException& exc);
	void warning(const SAXParseException& exc);
	string getExceptionsAsString();
};

#endif /* C_PARSERERRORHANDLER_H_ */
