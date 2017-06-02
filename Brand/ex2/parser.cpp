#include <cstdio>
#include <string>
#include <iostream>

#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/util/XMLString.hpp>

using namespace std;
using namespace xercesc;

string validationDoc = "measured-1-1-0.xsd";
char* valDoc_aschar = strcpy((char*)malloc(validationDoc.length()+1), validationDoc.c_str());

void createCSVOutput(XercesDOMParser *parser, DOMDocument *myDoc, DOMElement *root, DOMTreeWalker *walker){
	DOMNode *curNode = NULL;


	for(curNode = walker->getCurrentNode(); curNode != NULL; curNode = walker->nextNode()){
		string nodeName(XMLString::transcode(curNode->getNodeName()));
		if (0==nodeName.compare("amountOfPower"))
		{
			char * aop = XMLString::transcode(curNode->getAttributes()->getNamedItem(XMLString::transcode("value"))->getNodeValue());
			char * hour = XMLString::transcode(curNode->getParentNode()->getAttributes()->getNamedItem(XMLString::transcode("hour"))->getNodeValue());
			char * date = XMLString::transcode(curNode->getParentNode()->getParentNode()->getParentNode()->getAttributes()->getNamedItem(XMLString::transcode("date"))->getNodeValue());

			fprintf(stdout, "%s; %s; %s \n", date, hour, aop);
		}

	}
}

int main (int argc, char* args[]) {

	try {
		XMLPlatformUtils::Initialize();
	}
	catch (const XMLException& toCatch) {
		char* message = XMLString::transcode(toCatch.getMessage());
		cout << "Error during initialization! :\n"
				<< message << "\n";
		XMLString::release(&message);
		return 1;
	}

	XercesDOMParser* parser = new XercesDOMParser();
	parser->setValidationScheme(XercesDOMParser::Val_Always);
	parser->setDoNamespaces(true);    // optional

	ErrorHandler* errHandler = (ErrorHandler*) new HandlerBase();
	parser->setErrorHandler(errHandler);

	//LoadSchemeFile
	parser->loadGrammar(valDoc_aschar, Grammar::SchemaGrammarType);

	try {
		parser->parse(args[1]);
//		fprintf(stdout, "Parsing complete. \n");
	}
	catch (const XMLException& toCatch) {
		char* message = XMLString::transcode(toCatch.getMessage());
		fprintf(stderr, "%s %s %s", "Exception message is: \n", message, "\n");
		XMLString::release(&message);
		return -1;
	}
	catch (const DOMException& toCatch) {
		char* message = XMLString::transcode(toCatch.msg);
		fprintf(stderr, "%s %s %s", "Exception message is: \n", message, "\n");
		XMLString::release(&message);
		return -1;
	}
	catch (const SAXParseException& toCatch) {
		char* message = XMLString::transcode(toCatch.getMessage());
		fprintf(stderr, "%s %s %s", "Exception message is: \n", message, "\n");
		XMLString::release(&message);
		return -1;
	}
	catch (...) {
		fprintf(stderr, "Unexpected exception.");
		//return -1;
	}



	if(0==parser->getErrorCount())	fprintf(stdout, "Validation successful. \n");
	else							fprintf(stderr, "Validation faied. \n");




	DOMDocument *myDoc = parser->getDocument();
	DOMElement *root = myDoc->getDocumentElement();
	DOMTreeWalker *walker = myDoc->createTreeWalker(root,DOMNodeFilter::SHOW_ALL,NULL,true);


	createCSVOutput(parser, myDoc, root, walker);

	delete parser;
	delete errHandler;
	return 0;
}


