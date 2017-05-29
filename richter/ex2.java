import java.io.File;
import java.io.IOException;
import java.util.ArrayList;

import javax.xml.XMLConstants;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.transform.stream.StreamSource;
import javax.xml.validation.Schema;
import javax.xml.validation.SchemaFactory;
import javax.xml.validation.Validator;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;


/**
 * Advanced Programming: Exercise 2 (handling xml files)
 * @author Maximilian Richter
 */

public class ex2 {

	public static void main(String[] args) {

		try {
			
			System.out.println(readXmlToString(args[0]));
			
		}
		
		catch (ParserConfigurationException | SAXException e) {
			
			System.err.println("XML parsing failed.");
			
		}
		
		catch (IOException e) {
			
			System.err.println("Could not open file, check argument.");
			
		}
		
		try {
			
			validXmlSchema(args[0], "measured-1-1-0.xsd");
			
		}
		
		catch(IOException e){
			
			System.err.println("Could not find schema file.");
			
		}
		
		catch (SAXException e) {
			
			System.err.println("XML schema validation failed.");
			
		}
		
	}
	
	/**
	 * This function checks a given xml file against a xml schema.
	 * @param xmlFile Specifies the xml file given in String format.
	 * @param schemaName Specifies the schema file given in String format.
	 * @throws SAXException Parsing Error.
	 * @throws IOException File Exception (should not be thrown)
	 */
	public static void validXmlSchema(String xmlFile, String schemaName) throws SAXException, IOException{
		
		SchemaFactory factory = SchemaFactory.newInstance(XMLConstants.W3C_XML_SCHEMA_NS_URI);
		File schemaFile = new File(schemaName);
		Schema schema = factory.newSchema(schemaFile);
		Validator validator = schema.newValidator();
		validator.validate(new StreamSource(new File(xmlFile)));
		
	}
	
	/**
	 * This function reads the file indicated by fileName and returns a String of its contents.
	 * @param fileName Specifies the file name of the input file.
	 * @return Xml file as String.
	 * @throws ParserConfigurationException Parsing Error.
	 * @throws SAXException Parsing Error.
	 * @throws IOException Given input fileName does not specify a valid file.
	 */
	public static String readXmlToString(String fileName) throws ParserConfigurationException, SAXException, IOException{
		
		String returnString = "";
		
		File inputFile = new File(fileName);
		DocumentBuilderFactory dbFactory = DocumentBuilderFactory.newInstance();
		DocumentBuilder dBuilder = dbFactory.newDocumentBuilder();
		Document doc = dBuilder.parse(inputFile);
		doc.getDocumentElement().normalize();
		
		NodeList gasDayList = doc.getElementsByTagName("gasDay");
		
		for(int i = 0; i < gasDayList.getLength(); i++){
			
			Element gasDay = (Element) gasDayList.item(i);
			String date = gasDay.getAttribute("date");
			NodeList childNodes = gasDay.getChildNodes();
			ArrayList<Node> boundaryNodeList = new ArrayList<Node>();
			
			for(int j = 0; j < childNodes.getLength(); j++){
				
				Node node = childNodes.item(j);
				if(node.getNodeName().equals("boundaryNode")){
					boundaryNodeList.add(node);
				}
				
			}
				
			ArrayList<Node> timeList = new ArrayList<Node>();
			
			for(Node boundaryNode : boundaryNodeList){
				
				NodeList boundaryNodeChildren = boundaryNode.getChildNodes();
				
				for(int k = 0; k < boundaryNodeChildren.getLength(); k++){
					
					if(boundaryNodeChildren.item(k).getNodeName().equals("time")){
						timeList.add(boundaryNodeChildren.item(k));
					}
					
				}
				
			}
			
			for(Node timeNode : timeList){
				
				Element timeElement = (Element) timeNode;
				String hour = timeElement.getAttribute("hour");
				
//				// append hour if necessary to have the correct HH format
				if(hour.length() == 1)
					hour = "0" + hour;
				
				NodeList timeChildren = timeNode.getChildNodes();
				ArrayList<Node> valueNodes = new ArrayList<Node>();
				
				for(int l = 0; l < timeChildren.getLength(); l++){
					
					if(timeChildren.item(l).getNodeName().equals("amountOfPower")){
						valueNodes.add(timeChildren.item(l));
					}
					
				}
				
				for(Node valueNode : valueNodes){
					
					Element valueElement = (Element) valueNode;
					String value = valueElement.getAttribute("value");
					returnString += date + "; " + hour + "; " + value + System.getProperty("line.separator");
					
				}
				
			}
			
		}
		
		return returnString;
		
	}

}
