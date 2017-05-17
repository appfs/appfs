
#ifndef DOCUMENTERRORHANDLER_H_
#define DOCUMENTERRORHANDLER_H_

#include <xercesc/sax/HandlerBase.hpp>
#include <iostream>
#include <sstream>

class DocumentErrorHandler: public xercesc_3_1::HandlerBase {
public:
	DocumentErrorHandler();
	void error(const xercesc_3_1::SAXParseException&);
	void fatalError(const xercesc::SAXParseException&);
	void warning(const xercesc::SAXParseException&);
};

#endif /* DOCUMENTERRORHANDLER_H_ */
