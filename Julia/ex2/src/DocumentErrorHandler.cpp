/*
 * DocumentErrorHandler.cpp
 *
 *  Created on: May 8, 2017
 *      Author: july
 */

#include "DocumentErrorHandler.h"
#include <iostream>

using namespace std;

DocumentErrorHandler::DocumentErrorHandler() {
	// TODO Auto-generated constructor stub

}

void DocumentErrorHandler::error(const xercesc_3_1::SAXParseException &e){
	cerr << "Error occured: " << e.getMessage() << endl;
}

void DocumentErrorHandler::fatalError(const xercesc_3_1::SAXParseException &e){
	cerr << "Fatal error occured: " << e.getMessage() << endl;
}

void DocumentErrorHandler::warning(const xercesc_3_1::SAXParseException &e){
	cerr << "Warning: " << e.getMessage() << endl;
}
