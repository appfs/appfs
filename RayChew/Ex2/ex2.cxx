/**
 * \file ex2.cxx
 * \author Ray Chew
 * \date 5 June 2017
 * \brief XML Parser and Validator for measured-*.xml
 */

/**
 * \mainpage Ex2
 * 
 * \section Description
 * 
 * XML Parser and Validator for `measured-*.xml`. 
 * Reads XML file and writes the day, hour and amount of power to file `csv.txt`.<br>
 * 
 * * namespace-location pair: `"http://gaslab.zib.de/kwpt/measured ./measured-1-1-0.xsd"`<br>
 * * compile: `g++ -std=c++11 -O3 ex2.cxx -o ex2 -lxerces-c`<br>
 * * run: `./ex2 filepath/file.xml [-v]`<br>
 * * validation: enable `-V` flag
 */

/* -- Includes -- */
/* Xerus includes. */
#include <xercesc/parsers/SAXParser.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/util/XMLString.hpp>

/* C++ includes. */
#include <iostream> /* for std::cout, std::ofstream */
#include <fstream> /* for fstream::app */

using namespace std;
using namespace xercesc;

/**
 * \class ParserDocHandler
 * 
 * \brief Parses the XML document.
 * 
 * \details The ParserDocHandler class inheirts the HandlerBase
 * class of Xerus, which does nothing to a parsed document.
 * Class reads XML document and get date under gasDay,
 * 
 */
class ParserDocHandler : public HandlerBase {
  private:
  int trigger = 0; ///< if trigger == 1, there are changes worth writing to csv.txt. Else do nothing.
  string dd; ///< variable to store date.
  string hh; ///< variable to store hour.
  string vv; ///< variable to store values.
  string csvFileName = "csv.txt"; ///< filename of csv file.
  XMLCh* gasDay = XMLString::transcode("gasDay"); ///< XMLCh* type variable of string "gasDay" for parser.
  XMLCh* date = XMLString::transcode("date"); ///< XMLCh* type variable of string "date" for parser.
  XMLCh* time = XMLString::transcode("time"); ///< XMLCh* type variable of string "time" for parser.
  XMLCh* hour = XMLString::transcode("hour"); ///< XMLCh* type variable of string "hour" for parser.
  XMLCh* amtP = XMLString::transcode("amountOfPower"); ///< XMLCh* type variable of string "amountOfPower" for parser.
  XMLCh* value = XMLString::transcode("value"); ///< XMLCh* type variable of string "value" for parser.
  
  ofstream csv;
  
  /** 
   *  \brief A method to make the "hour" value double digits.
   *  \param HH cast as string
   *  \return Returns 01, 02, ... , 24.
   */
  string doubleHH(string HH) // function to make hour double digits.
  {
    if (HH.length() == 1) {
      HH = "0" + HH;
    }
    return HH;
  }
  
  public:
    /// Creates an instance of the ParserDocHandler class.
    ParserDocHandler()
    {
      cout << "Start Parser..." << endl;
    }
    
    /** 
     * \brief Method that writes day, hour and value from parsed data to file.
     * 
     * \param name the element name to be compared.
     * \param atts the list of attributes.
     * 
     * \return None.
     * 
     */
    void startElement(const XMLCh* const name, AttributeList& atts)
    {
      if (XMLString::equals(name, gasDay)) { // compare element name. If name is correct, get respective attribute values and store.
	char* message = XMLString::transcode(atts.getValue(date));
	dd = message;
	//cout << dd << endl;
	XMLString::release(&message);
      }
      else if (XMLString::equals(name, time)) {
	char* message = XMLString::transcode(atts.getValue(hour));
	hh = message;
	//cout << hh << endl;
	XMLString::release(&message);
      }
      else if (XMLString::equals(name, amtP)) {
	char* message = XMLString::transcode(atts.getValue(value));
	vv = message;
	//cout << vv << endl;
	XMLString::release(&message);
	trigger = 1; // change in amount of power:value denotes a change worth storing. Set trigger to 1.
      }
      if (trigger == 1) { // if there is a change needed to write into csv.txt, write and output line.
	csv.open(csvFileName, fstream::app);
	csv << dd << "; " << doubleHH(hh) << "; " << vv << "\n";
	csv.close();
	cout << dd << "; " << doubleHH(hh) << "; " << vv << "\n";
	trigger = 0;
      }
    }
    
    /// Deconstructor of the ParserDocHandler class.
    ~ParserDocHandler()
    {
      cout << "Write to CSV completed. Values were written to " << csvFileName << "." << endl;
      XMLString::release(&gasDay);
      XMLString::release(&date);
      XMLString::release(&time);
      XMLString::release(&hour);
      XMLString::release(&amtP);
      XMLString::release(&value);
    }
};

/**
 * \class ParserErrorHandler
 * 
 * \brief A basic (and default) version of error handling
 * for an XML document provided by Xerces-C.
 * 
 * \details Prints error messages to stderr along with
 * line and column numbers. It inherits the ErrorHandler
 * class provided by Xerces-c.
 * 
 */
class ParserErrorHandler : public ErrorHandler { // error handler.
private:
  /// Method that prints error messages along with line and col.
  void reportParseException(const SAXParseException& ex)
  {
    char* msg = XMLString::transcode(ex.getMessage());
    fprintf(stderr, "line #%llu column #%llu: %s.\n", ex.getLineNumber(), ex.getColumnNumber(), msg);
    XMLString::release(&msg);
  }
  
public:
  /// Call reportParseException for warnings.
  void warning(const SAXParseException& ex)
  {
    reportParseException(ex);
  }
  /// Call reportParseException for errors.
  void error(const SAXParseException& ex)
  {
    reportParseException(ex);
  }
  /// Call reportParseException for fatal errors.
  void fatalError(const SAXParseException& ex)
  {
    reportParseException(ex);
  }
  /// Do nothing for reset errors.
  void resetErrors()
  {
    ;
  }
};

int main(int argc, char* argv[])
{
  const char* xmlFile = argv[1]; ///< Take xml file as argument of console input.
  
  /*
   * Initialize Xerces to parse document.
   */
  XMLPlatformUtils::Initialize();
  {
    SAXParser saxParser; /// initializer Parser.
    
    ParserErrorHandler parserErrorHandler; /// initialize ErrorHandler.
    
    saxParser.setErrorHandler(&parserErrorHandler); /// set the ErrorHandler.
    saxParser.setValidationScheme(SAXParser::Val_Auto); /// Validate if external schema is found.
    saxParser.setDoNamespaces(true); /// "When set to true, parser starts enforcing all the constraints/rules specified by the NameSpace specification."
    saxParser.setDoSchema(false); /// "This method allows users to enable or disable the parser's schema processing. When set to false, parser will not process any schema found."
    saxParser.setValidationSchemaFullChecking(true); /// Do full schema checking.
    
    ParserDocHandler docHandler; /// initialize DocumentHandler.
    saxParser.setDocumentHandler(&docHandler); /// set the DocumentHandler.
    
    saxParser.parse(xmlFile); /// Parse file.
    
    /*
     * if -V flag is present, do validation.
     */
    for(int i = 0; i < argc; i++) {
      if(string(argv[i]) == "-V") {
	XMLCh const* location = XMLString::transcode("http://gaslab.zib.de/kwpt/measured ./measured-1-1-0.xsd");
	saxParser.setExternalSchemaLocation(location);
	cout << endl << "Using Namespace-Location pair: \"" << XMLString::transcode(saxParser.getExternalSchemaLocation()) << "\"" << endl << endl;
	
	cout << "Start Validator..." << endl;
	HandlerBase docHandler;
	saxParser.setDocumentHandler(&docHandler);
	saxParser.setDoSchema(true);
	saxParser.parse(xmlFile); // Validate xml.
	
	if (saxParser.getErrorCount() != 0)
	  fprintf(stderr, "XML file does not conform to schema.\n");
	
	cout << "Validation completed." << endl << endl;
      }
    }
  }
  XMLPlatformUtils::Terminate(); /// terminate Parser.
  return 0;
}

