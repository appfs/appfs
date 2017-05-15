
#ifndef DOCUMENTERRORHANDLER_H_
#define DOCUMENTERRORHANDLER_H_

#include <xercesc/sax/HandlerBase.hpp>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>

class DocumentErrorHandler: public xercesc_3_1::HandlerBase {
public:
	DocumentErrorHandler();
	void error(const xercesc_3_1::SAXParseException&);
	void fatalError(const xercesc::SAXParseException&);
	void warning(const xercesc::SAXParseException&);
	std::string getInformationsAsString();

private:
	std::vector<std::string> errors;
	std::vector<std::string> fatalErrors;
	std::vector<std::string> warnings;
	void addInformation(std::vector<std::string>, const xercesc_3_1::SAXParseException&);
	void addStrings(std::stringstream&, std::vector<std::string>);
};

#endif /* DOCUMENTERRORHANDLER_H_ */
