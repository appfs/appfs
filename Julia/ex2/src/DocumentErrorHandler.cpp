/*
 * DocumentErrorHandler.cpp
 *
 * 	\brief     Class for handle errors while parsing xml files.
 *  \details   Extends xercesc_3_1::HandlerBase for error handling while parsing xml files
 *  \author    Julia Baumbach
 *  \date      01.05.2017
 */

#include "DocumentErrorHandler.h"

using namespace std;
using namespace xercesc;

/**
 * \fn DocumentErrorHandler::DocumentErrorHandler()
 * \brief Constructor, initializes a new DocumentErrorHandler instance
 */
DocumentErrorHandler::DocumentErrorHandler() {
	// Nothing to do
}

/**
 * \fn void DocumentErrorHandler::error(const SAXParseException &ex)
 * \brief shows the error message and line number of an error which was found
 * \param ex the exception which was thrown
 */
void DocumentErrorHandler::error(const SAXParseException &ex){
	HandlerBase::error(ex);
	cerr << "Error found: " << XMLString::transcode(ex.getMessage()) <<" at line " << ex.getLineNumber() << endl;
}

/**
 * \fn void DocumentErrorHandler::fatalError(const SAXParseException &ex)
 * \brief shows the error message and line number of an fatal error which was found
 * \param ex the exception which was thrown
 */
void DocumentErrorHandler::fatalError(const SAXParseException &ex){
	HandlerBase::fatalError(ex);
	cerr << "Fatal error found: " << XMLString::transcode(ex.getMessage()) <<" at line " << ex.getLineNumber() << endl;
}

/**
 * \fn void DocumentErrorHandler::warning(const SAXParseException &ex)
 * \brief shows the warning message and line number of warning which was found
 * \param ex the exception which was thrown
 */
void DocumentErrorHandler::warning(const SAXParseException &ex){
	HandlerBase::warning(ex);
	cerr << "Warning found: " << XMLString::transcode(ex.getMessage()) <<" at line " << ex.getLineNumber() << endl;
}
