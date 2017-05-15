#include <xercesc/dom/DOM.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMDocumentType.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMImplementationLS.hpp>
#include <xercesc/dom/DOMNodeIterator.hpp>
#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/dom/DOMText.hpp>
#include <xercesc/dom/DOMTreeWalker.hpp>
#include <xercesc/parsers/XercesdomParser.hpp>
#include <xercesc/util/XMLUni.hpp>

#include <string>
#include <stdexcept>

#include <iostream>

using namespace std;
using namespace xercesc;

class SimpleErrorHandler : public ErrorHandler
{
private:
	void printError(const SAXParseException& ex)
	{
		char* message = XMLString::transcode(ex.getMessage());
		//fprintf(stderr, "Error: Line %llu, column %llu: %s\n", ex.getColumnNumber(), ex.getLineNumber(), message);
		XMLString::release(&message);
	}

public:
	void error(const SAXParseException& ex)
	{
		printError(ex);
	}

	void fatalError(const SAXParseException& ex)
	{
		printError(ex);
	}

	void resetErrors()
	{
	}

	void warning(const SAXParseException& ex)
	{
		printError(ex);
	}
};

int main(int argc, const char *argv[])
{
	if (2 != argc)
	{
		fprintf(stderr, "ERROR! Correct usage: ex2 <xml file>\n");
		return -1;
	}

	XMLPlatformUtils::Initialize();

	XercesDOMParser *domParser = new XercesDOMParser();
	if (NULL == domParser->loadGrammar("measured-1-1-0.xsd", Grammar::SchemaGrammarType))
	{
		fprintf(stderr, "Unable to load schema measured-1-1-0.xsd\n");
		return 0;
	}

	SimpleErrorHandler parserErrorHandler;

	domParser->setErrorHandler(&parserErrorHandler);
	domParser->setValidationScheme(XercesDOMParser::Val_Always);
	domParser->setDoNamespaces(true);
	domParser->setDoSchema(true);
	//domParser->setValidationConstraintFatal(true);

	domParser->parse(argv[1]);
	if (0 == domParser->getErrorCount())
	{
		fprintf(stderr, "Validated sucessfully.\n");
	}
	else
	{
		fprintf(stderr, "Validated NOT sucessfully.\n");
	}

	DOMDocument *document = domParser->getDocument();
	DOMElement *rootNode = document->getDocumentElement();
	DOMTreeWalker *treeWalker =	document->createTreeWalker(rootNode, DOMNodeFilter::SHOW_ALL, NULL, true);
	DOMNode *currentNode = NULL;

	for (currentNode = treeWalker->getCurrentNode(); currentNode != NULL; currentNode = treeWalker->nextNode())
	{
		string str(XMLString::transcode(currentNode->getNodeName()));
		if (str.compare("amountOfPower")==0)
		{
			char * amountOfPowerValue = XMLString::transcode(currentNode->getAttributes()->getNamedItem(XMLString::transcode("value"))->getNodeValue());
			char * hour = XMLString::transcode(currentNode->getParentNode()->getAttributes()->getNamedItem(XMLString::transcode("hour"))->getNodeValue());
			char * date = XMLString::transcode(currentNode->getParentNode()->getParentNode()->getParentNode()->getAttributes()->getNamedItem(XMLString::transcode("date"))->getNodeValue());
			fprintf(stdout, "%s; %s; %s\n", date, hour, amountOfPowerValue);
		}
	}

	delete domParser;
	XMLPlatformUtils::Terminate();
	return 0;
}
