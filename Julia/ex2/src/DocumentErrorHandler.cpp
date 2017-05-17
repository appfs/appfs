#include "DocumentErrorHandler.h"

using namespace std;
using namespace xercesc;

DocumentErrorHandler::DocumentErrorHandler() {
	// Nothing to do
}

void DocumentErrorHandler::error(const SAXParseException &ex){
	HandlerBase::error(ex);
	cerr << "Error found: " << XMLString::transcode(ex.getMessage()) <<" at line " << ex.getLineNumber() << endl;
}

void DocumentErrorHandler::fatalError(const SAXParseException &ex){
	HandlerBase::fatalError(ex);
	cerr << "Fatal error found: " << XMLString::transcode(ex.getMessage()) <<" at line " << ex.getLineNumber() << endl;
}

void DocumentErrorHandler::warning(const SAXParseException &ex){
	HandlerBase::warning(ex);
	cerr << "Warning found: " << XMLString::transcode(ex.getMessage()) <<" at line " << ex.getLineNumber() << endl;
}
