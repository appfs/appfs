#include <xercesc/parsers/SAXParser.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/util/XMLString.hpp>

#include <iostream>
#include <fstream>

using namespace std;
using namespace xercesc;

class ParserDocHandler : public HandlerBase { // inherit HandlerBase, add methods to parse xml and store lines in csv.txt.
private:
    int trigger = 0; // f trigger == 1, there are changes worth writing to csv.txt. Else do nothing.
    string dd, hh, vv; // define variables to store date, hour and values.
    string csvFileName = "csv.txt"; // define filename of csv file.
    XMLCh* gasDay = XMLString::transcode("gasDay"); // define values for parser, for comparison.
    XMLCh* date = XMLString::transcode("date");
    XMLCh* time = XMLString::transcode("time");
    XMLCh* hour = XMLString::transcode("hour");
    XMLCh* amtP = XMLString::transcode("amountOfPower");
    XMLCh* value = XMLString::transcode("value");

    ofstream csv;

    string doubleHH(string HH) // function to make hour double digits.
    {
        if (HH.length() == 1) {
            HH = "0" + HH;
        }
        return HH;
    }

public: // document handler for parser. Using SAX model.
    ParserDocHandler()
    {
        cout << "Start Parser..." << endl;
    }

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

class ParserErrorHandler : public ErrorHandler { // error handler.
private:
    void reportParseException(const SAXParseException& ex)
    {
        char* msg = XMLString::transcode(ex.getMessage());
        fprintf(stderr, "line #%llu column #%llu: %s.\n", ex.getLineNumber(), ex.getColumnNumber(), msg);
        XMLString::release(&msg);
    }

public:
    void warning(const SAXParseException& ex)
    {
        reportParseException(ex);
    }
    void error(const SAXParseException& ex)
    {
        reportParseException(ex);
    }
    void fatalError(const SAXParseException& ex)
    {
        reportParseException(ex);
    }
    void resetErrors()
    {
        ;
    }
};

int main(int argc, char* argv[])
{
    const char* xmlFile = argv[1]; // take xml file as argument.
    string input;

    XMLPlatformUtils::Initialize(); // initialize Xerces.
    {
        SAXParser saxParser; // initializer Parser.

        ParserErrorHandler parserErrorHandler; // initialize ErrorHandler.

        saxParser.setErrorHandler(&parserErrorHandler); // set the ErrorHandler.
        saxParser.setValidationScheme(SAXParser::Val_Auto); // Validate if external schema is found.
        saxParser.setDoNamespaces(true); // "When set to true, parser starts enforcing all the constraints/rules specified by the NameSpace specification."
        saxParser.setDoSchema(false); // "This method allows users to enable or disable the parser's schema processing. When set to false, parser will not process any schema found."
	saxParser.setValidationSchemaFullChecking(true);

        ParserDocHandler docHandler; // initialize DocumentHandler.
        saxParser.setDocumentHandler(&docHandler); // set the DocumentHandler.

        saxParser.parse(xmlFile); // Parse file.
	
	cout << endl << "(: Would you like to validate the file? (Y/N) ";
	getline(cin,input);
	if (( input == "Y") || (input == "y")){
	XMLCh const* location = XMLString::transcode("http://gaslab.zib.de/kwpt/measured ./measured-1-1-0.xsd");
    	saxParser.setExternalSchemaLocation(location);
	cout << "Using Namespace-Location pair: \"" << XMLString::transcode(saxParser.getExternalSchemaLocation()) << "\"" << endl << endl;
	
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
    XMLPlatformUtils::Terminate(); // terminate Parser.
    return 0;
}