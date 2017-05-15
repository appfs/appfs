#include "DocumentErrorHandler.h"

using namespace std;
using namespace xercesc;

DocumentErrorHandler::DocumentErrorHandler() {
	errors = vector<string>();
	fatalErrors = vector<string>();
	warnings = vector<string>();
}

void DocumentErrorHandler::error(const SAXParseException &ex){
	HandlerBase::error(ex);
	addInformation(errors, ex);
}

void DocumentErrorHandler::fatalError(const SAXParseException &ex){
	HandlerBase::fatalError(ex);
	addInformation(fatalErrors, ex);
}

void DocumentErrorHandler::warning(const SAXParseException &ex){
	HandlerBase::warning(ex);
	addInformation(warnings, ex);
}

void DocumentErrorHandler::addInformation(vector<string> vec, const SAXParseException &ex){
	stringstream s;
	s <<  "Information: " << XMLString::transcode(ex.getMessage()) << " at line " << ex.getLineNumber();
	vec.push_back(s.str());
}

string DocumentErrorHandler::getInformationsAsString(){
	stringstream s;
	s << "Warnings found: (" << warnings.size() << ")" << endl;
	addStrings(s, warnings);
	s << "Errors found: (" << errors.size() << ")" << endl;
	addStrings(s, errors);
	s << "Fatal errors found: (" << fatalErrors.size() << ")" << endl;
	addStrings(s, fatalErrors);

	return s.str();
}

void DocumentErrorHandler::addStrings(stringstream& s, vector<string> vec){
	for(unsigned int i = 0; i<vec.size(); i++){
		s << vec.at(i) << endl;
	}
}
