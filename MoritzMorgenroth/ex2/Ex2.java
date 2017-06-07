import java.io.*;
import java.lang.*;
import javax.xml.transform.stream.StreamSource;
import javax.xml.validation.Schema;
import javax.xml.validation.SchemaFactory;
import javax.xml.validation.Validator;
import javax.xml.XMLConstants;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.transform.Result;
import javax.xml.transform.Source;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;
import javax.xml.transform.stream.StreamSource;

import org.w3c.dom.Document;



public class Ex2{

	public static void main(String[]args){

		System.out.println("Running Ex2... ");

		// check if schema is explicitly defined in input arg, else use default
		String schemaFileName = "measured-1-1-0.xsd";
		try{
			schemaFileName = args[1];
		}
		catch(Exception e){
			System.out.println("Schema file not explicitly given, defaulting to measured-1-1-0.xsd");
		}

		String xmlFileName = "example.measured-1-1-0.xml"; 
		try{
			xmlFileName = args[0];
		}
		catch(Exception e){
			System.out.println("XML file not explicitly given, defaulting to example.measured-1-1-0.xml");
		}
		System.out.println("Reading " + xmlFileName + " with schema "+ schemaFileName + "... ");

		try{
			System.out.println("Validating schema...");
			validateWithSchema(xmlFileName, schemaFileName);
		}
		catch(Exception e){
			System.out.println("An error occured validating the schema:");
			System.err.println(e.toString());
		}
		try{
			System.out.println("Parsing to csv...");
			parseToCsv(xmlFileName);
		}
		catch(Exception e){
			System.out.println("An error occured parsing to csv:");
			System.err.println(e.toString());
		}

		System.out.println("Terminating Ex2...");
	}

	public static void validateWithSchema(String xmlFileName, String schemaFileName) throws Exception{
		StreamSource xmlFile = new StreamSource(new File(xmlFileName));
		File schemaFile = new File(schemaFileName);
		SchemaFactory schemaFactory = SchemaFactory.newInstance(XMLConstants.W3C_XML_SCHEMA_NS_URI);
		Schema schema = schemaFactory.newSchema(schemaFile);
	    Validator validator = schema.newValidator();
	  	validator.validate(xmlFile);
	}

	public static void parseToCsv(String xmlFileName) throws Exception{
		File stylesheet = new File("style.xsl");
		File xmlSource = new File(xmlFileName);

		DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
		DocumentBuilder builder = factory.newDocumentBuilder();
		Document document = builder.parse(xmlSource);

		StreamSource stylesource = new StreamSource(stylesheet);
		Transformer transformer = TransformerFactory.newInstance()
		        .newTransformer(stylesource);
		Source source = new DOMSource(document);
		String outputFileName = xmlFileName.replace(".xml", "") + ".csv";
		System.out.println("Writing to " + outputFileName);
		Result outputTarget = new StreamResult(new File(outputFileName));
		transformer.transform(source, outputTarget);		        
	}

}