/*
 * ex2.cpp
 *
 * author: Felix Baumann
 *
 * Requirements: External library Xerces (xml parser) must be installed
 *
 * Compile: g++ ex2.cpp -lxerces-c -o ex2
 *
 * Run for output to console:   ./ex2 xmlFileName.xml schemaFileName.xsd
 *
 * Run for output to out.csv:  ./ex2 xmlFileName.xml schemaFileName.xsd >out.csv
 *
 *
 */


#include <string>
#include <iostream>
#include <stdexcept>
#include <stdio.h>

//includes for external XML-library Xerces
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>


using namespace xercesc;
using namespace std;



int main(int argc, char *argv[]){

  if( argc != 3){
    cout << "Call the program as: "
         << argv[0] << " filename.xml "
         << argv[1] << " schema.xsd" << endl;
    exit(EXIT_FAILURE);
  }


  //initializing XML-library xerces
  try{
    XMLPlatformUtils::Initialize();

  }catch ( const XMLException& e ){
    char* message = XMLString::transcode( e.getMessage() );
    fprintf(stderr, "%s, %p\n", "Error during initialization of Xerces-library: ", message);
    return -1;
  }


  //creating DOM parser object
  XercesDOMParser* xmlParser = new XercesDOMParser();

  //load schema from argument list and add it to the grammar of xmlParser
  if (xmlParser->loadGrammar(argv[2], Grammar::SchemaGrammarType) == NULL){
    fprintf(stderr, "%s\n", "couldn't load schema. It should be given as second argument when calling the program");
  }

  // Configure DOM parser
  xmlParser->setValidationScheme (XercesDOMParser::Val_Always); //no validation
  xmlParser->setDoNamespaces     (true);
  xmlParser->setDoSchema         (true);                       //validate against Schema




  //parsing xml document
  try{
    xmlParser->parse(argv[1]);

  } catch (const XMLException& e){
    char* message = XMLString::transcode( e.getMessage() );
    fprintf(stderr, "%s, %p\n", "XML-error while parsing: ", message);
    return -1;


  } catch (const DOMException& e){
    char* message = XMLString::transcode( e.msg );
    fprintf(stderr,"%s, %p\n", "DOM-error while parsing: ", message);
    return -1;

  } catch (...) {
    fprintf(stderr,"%s\n", "Unexpected exception");
    return -1;
  }


  //settings to "walk" through DOM-tree structure
  DOMDocument* domDoc       = xmlParser->getDocument();
  DOMElement* root          = domDoc   ->getDocumentElement();
  DOMTreeWalker* treeWalker = domDoc   ->createTreeWalker (root, DOMNodeFilter::SHOW_ALL, NULL, true);

  DOMNode* currNode         = treeWalker->getCurrentNode();

  //walk through all node
  while( currNode != NULL ){

    string name(XMLString::transcode(currNode->getNodeName()));

    if(name == "amountOfPower"){
      char* amountOfPower = XMLString::transcode(currNode->getAttributes()
                                                         ->getNamedItem(XMLString::transcode("value"))
                                                         ->getNodeValue());

      char* date          = XMLString::transcode(currNode->getParentNode()
                                                         ->getParentNode()
                                                         ->getParentNode()
                                                         ->getAttributes()
                                                         ->getNamedItem(XMLString::transcode("date"))
                                                         ->getNodeValue());

      char* hour          = XMLString::transcode(currNode->getParentNode()
                                                         ->getAttributes()
                                                         ->getNamedItem(XMLString::transcode("hour"))
                                                         ->getNodeValue());

      fprintf(stdout, "%s; %s; %s\n",  date, hour, amountOfPower);
    }

    //go to next node
    currNode = treeWalker-> nextNode();
  }



  delete xmlParser;

  try{
    XMLPlatformUtils::Terminate();

  }catch ( const XMLException& e ){

    char* message = xercesc::XMLString::transcode( e.getMessage() );
    fprintf(stderr, "%s %p\n", "XML toolkit terminating error: ", message);
  }

  return 0;

}
