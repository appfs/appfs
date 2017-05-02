//
// in XML file, xsi:schemaLocation="http://gaslab.zib.de/kwpt/measured ../schemes/measured-1-1-0.xsd"
// is changed to xsi:schemaLocation="http://gaslab.zib.de/kwpt/measured measured-1-1-0.xsd"
//
#include <xercesc/parsers/SAXParser.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/util/XMLString.hpp>

#include <iostream>
#include <fstream>

using namespace std;
using namespace xercesc;

class ParserDocHandler : public HandlerBase { // inherit HandlerBase, add methods to parse xml and store lines in csv.txt.
private: 
  int trigger = 0; // define variables. If trigger == 1, there are changes worth writing to csv.txt. Else do nothing.
  string dd, hh, vv; // define variables to store date, hour and values.
  XMLCh* gasDay = XMLString::transcode("gasDay"); // define values for parser, for comparison. 
  XMLCh* date = XMLString::transcode("date");
  XMLCh* time = XMLString::transcode("time");
  XMLCh* hour = XMLString::transcode("hour");
  XMLCh* amtP = XMLString::transcode("amountOfPower");
  XMLCh* value = XMLString::transcode("value");
  
  ofstream csv; // initialize write.
  
  string doubleHH(string HH) { // function to make hour double digits.
    if (HH.length() == 1){
      HH = "0"+HH;
    }
    return HH;
  }
  
  public: // document handler for parser. Using SAX model.
    void startElement(const XMLCh* const name,AttributeList& atts) {
      
      if (XMLString::equals(name,gasDay)) { // compare element name. If name is correct, get respective attribute values and store.
	char* message = XMLString::transcode(atts.getValue(date));
	dd = message;
	//cout << dd << endl;
	XMLString::release(&message);
      }
      else if (XMLString::equals(name,time)) {
	char* message = XMLString::transcode(atts.getValue(hour));
	hh = message;
	//cout << hh << endl;
	XMLString::release(&message);
      }
      else if (XMLString::equals(name,amtP)) {
	char* message = XMLString::transcode(atts.getValue(value));
	vv = message;
	//cout << vv << endl;
	XMLString::release(&message);
	trigger = 1; // change in amount of power:value denotes a change worth storing. Set trigger to 1.
      }
      if (trigger==1) { // if there is a change needed to write into csv.txt, write and output line.
	csv.open ("csv.txt", fstream::app);
	csv << dd << "; " << doubleHH(hh) << "; " << vv <<"\n";
	csv.close();
	cout << dd << "; " << doubleHH(hh) << "; " << vv <<"\n";
	trigger=0;
      }
    }
    
    void fatalError(const SAXParseException& exception) { // error handling. Took it off Xerces documentation.
      char* message = XMLString::transcode(exception.getMessage());
      cout << "Fatal Error: " << message
      << " at line: " << exception.getLineNumber()
      << endl;
      XMLString::release(&message);
    }
    ~ParserDocHandler();
};

ParserDocHandler::~ParserDocHandler() {
  XMLString::release(&gasDay);
  XMLString::release(&date);
  XMLString::release(&time);
  XMLString::release(&hour);
  XMLString::release(&amtP);
  XMLString::release(&value);
}

class ParserErrorHandler : public ErrorHandler { // error handler. Took it off Xerces documentation.
private:
  void reportParseException(const SAXParseException& ex) {
    char* msg = XMLString::transcode(ex.getMessage());
    fprintf(stderr, "at line %llu column %llu, %s\n", 
	    ex.getLineNumber(), ex.getColumnNumber(), msg);
    XMLString::release(&msg);
  }
  
public:
  void warning(const SAXParseException& ex) {
    reportParseException(ex);
  }
  void error(const SAXParseException& ex) {
    reportParseException(ex);
  }
  void fatalError(const SAXParseException& ex) {
    reportParseException(ex);
  }
  void resetErrors() {;
  }
};

int main(int argc, char* argv[]) {
  const char* xmlFile = argv[1]; // take xml file as first argument.
  const char* xsdFile = argv[2]; // take xsd fle as second argument.
  
  try {
    XMLPlatformUtils::Initialize(); // initialize Xerces.
  }
  catch (const XMLException& toCatch) { // error handling. Took it off sample code in Xerces documentation.
    char* message = XMLString::transcode(toCatch.getMessage());
    cout << "Error during initialization! :\n"
    << message << "\n";
    XMLString::release(&message);
    return 1;
  }
  {
    SAXParser saxParser; // initializer Parser.
    
    if (saxParser.loadGrammar(xsdFile, Grammar::SchemaGrammarType)==NULL) { // check if xsd file is valid.
      printf("couldn't load schema\n");
    }
    
    ParserErrorHandler parserErrorHandler; // initialize ErrorHandler.
    
    saxParser.setErrorHandler(&parserErrorHandler); // set the ErrorHandler.
    saxParser.setValidationScheme(SAXParser::Val_Auto); // Validate if external schema is found.
    saxParser.setDoNamespaces(true); // "When set to true, parser starts enforcing all the constraints/rules specified by the NameSpace specification."
    saxParser.setDoSchema(true); // "This method allows users to enable or disable the parser's schema processing. When set to false, parser will not process any schema found."
    
    ParserDocHandler docHandler; // initialize DocumentHandler.
    saxParser.setDocumentHandler(&docHandler); // set the DocumentHandler.
    
    try {
      saxParser.parse(xmlFile); // Parse file.
    }
    catch (const XMLException& toCatch) { // Exception handling, took it off SAX parsing sample documentation.
      char* message = XMLString::transcode(toCatch.getMessage());
      cout << "Exception message is: \n"
      << message << "\n";
      XMLString::release(&message);
      return -1;
    }
    catch (const SAXParseException& toCatch) {
      char* message = XMLString::transcode(toCatch.getMessage());
      cout << "Exception message is: \n"
      << message << "\n";
      XMLString::release(&message);
      return -1;
    }
    catch (...) {
      cout << "Unexpected Exception \n" ;
      return -1;
    }
  }
  XMLPlatformUtils::Terminate();
  return 0;
}