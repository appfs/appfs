/*
 * DocumentErrorHandler.h
 *
 * 	\brief     Class for handle errors while parsing xml files.
 *  \details   Extends xercesc_3_1::HandlerBase for error handling while parsing xml files
 *  \author    Julia Baumbach
 *  \date      01.05.2017
 */

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
