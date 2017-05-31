#include <assert.h>
#include <stdio.h>

#include <libxml/parser.h>
#include <libxml/xmlschemas.h>
#include <libxml/valid.h>

#ifdef LIBXML_PUSH_ENABLED
#define BUFFER_SIZE 1024

void handleValidationError(void *ctx, const char *format, ...) {
	char *errMsg;
	va_list args;
	va_start(args, format);
	vasprintf(&errMsg, format, args);
	va_end(args);
	fprintf(stderr, "\nValidation Error: %s\n", errMsg);
	free(errMsg);
}


int read_packet(char * mem, int size, FILE * fp)
{
	int res;
	res = fread(mem, 1, size, fp);
	return(res);
}

xmlChar * gasDay, * boundaryNode, * time, * power, * pressure;
int count = 0;

int print_element_names(xmlNode * a_node, FILE * output)
{
	char * result;
	xmlNode *cur_node = NULL;

	result = calloc(264, sizeof(char));
	for (cur_node = a_node; cur_node; cur_node = cur_node->next) 
	{
		if (strcmp(cur_node->name, "gasDay") == 0) gasDay = xmlGetProp(cur_node, "date");
		else if (strcmp(cur_node->name, "boundaryNode") == 0) boundaryNode = xmlGetProp(cur_node, "id");
		else if (strcmp(cur_node->name, "time") == 0) time = xmlGetProp(cur_node, "hour");
		else if (strcmp(cur_node->name, "amountOfPower") == 0) power = xmlGetProp(cur_node, "value");
		else if (strcmp(cur_node->name, "simulatedPressure") == 0)
		{
			pressure = xmlGetProp(cur_node, "value");
			sprintf(result, "%s, %s, %s, %s, %s\n", gasDay, boundaryNode, time, power, pressure );
			fputs(result, output);
			count++;
		}
	        print_element_names(cur_node->children, output);
	}
	free(result);
	return 0;
}

int parse_xml(char * xml_path, char * xsd_path, FILE * fp)
{
	int res;
	char * buffer = NULL;
	xmlParserCtxtPtr ctxt = NULL;
	xmlDocPtr doc = NULL;

	xmlSchemaParserCtxtPtr parserCtxt = NULL;
	xmlSchemaPtr schema = NULL;
	xmlSchemaValidCtxtPtr validCtxt = NULL;

	FILE * output;

	parserCtxt = xmlSchemaNewParserCtxt(xsd_path);

	if (parserCtxt == NULL)
	{
		fprintf(stderr, "Could not create XSD schema parsing context.\n");
	}

	schema = xmlSchemaParse(parserCtxt);

	if (schema == NULL)
	{
		fprintf(stderr, "Could not parse XSD schema.\n");
	}
	validCtxt = xmlSchemaNewValidCtxt(schema);

	if (!validCtxt) {
		fprintf(stderr, "Could not create XSD schema validation context.\n");
	}

	xmlSetStructuredErrorFunc(NULL, NULL);
	xmlSetGenericErrorFunc(NULL, handleValidationError);
	xmlThrDefSetStructuredErrorFunc(NULL, NULL);
	xmlThrDefSetGenericErrorFunc(NULL, handleValidationError);

	buffer = calloc(BUFFER_SIZE, sizeof(char));

	res = read_packet(buffer, BUFFER_SIZE, fp);
	if (res <= 0) {
		fprintf(stderr, "Failed to parse %s\n", xml_path);
	}
	ctxt = xmlCreatePushParserCtxt(NULL, NULL, buffer, res, xml_path);
	if (ctxt == NULL) 
	{
		fprintf(stderr, "Failed to create parser context !\n");
	}
	while ((res = read_packet(buffer, BUFFER_SIZE, fp)) > 0)
	{
		xmlParseChunk(ctxt, buffer, res, 0);
	}
	// signal end of parsing
	xmlParseChunk(ctxt, buffer, 0, 1);

	doc = ctxt->myDoc;
	res = ctxt->wellFormed;

	if (!res) 
	{
		fprintf(stderr, "Failed to parse %s\n", xml_path);
	}
	res = xmlSchemaValidateDoc(validCtxt, doc);
	printf("Validation successful: %s\n", (res == 0) ? "YES" : "NO");
	output = fopen("output.txt", "w");
	fputs("DATE, ID, HOUR, POWER, PRESSURE\n", output);
	print_element_names(xmlDocGetRootElement(doc), output);
	printf("\n%d lines written to output.\n", count);
	fclose(output);

	xmlFreeParserCtxt(ctxt);
	xmlSchemaFreeParserCtxt(parserCtxt);
	xmlSchemaFree(schema);
	xmlSchemaFreeValidCtxt(validCtxt);

	free(buffer);
	xmlFreeDoc(doc);
}

int main (int argc, char * argv[]) 
{
	assert(argc > 1);
	char * xml_path = argv[1];
	char * xsd_path = NULL;
	FILE * fp;
	if(argc > 2) xsd_path = argv[2];

	LIBXML_TEST_VERSION

	fp = fopen(xml_path, "rb");
	if (fp)
	{
		parse_xml(xml_path, xsd_path, fp);
		fclose(fp);
	} else {
		fprintf(stderr, "Failed to parse %s\n", xml_path);
	}

	xmlCleanupParser();
	xmlMemoryDump();
	return 0;
}

#else /* ! LIBXML_PUSH_ENABLED */
int main(int argc, char **argv) {
	fprintf(stderr, "Library not compiled with push parser support\n");
	return(1);
}
#endif
