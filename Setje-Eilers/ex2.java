import javax.xml.XMLConstants;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.DocumentBuilder;
import org.w3c.dom.Document;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.NodeList;
import org.w3c.dom.Node;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import org.xml.sax.SAXException; 
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.transform.stream.StreamSource;
import javax.xml.validation.Schema;
import javax.xml.validation.SchemaFactory;
import javax.xml.validation.Validator;

public class ex2{
	
	public static void validateFile(File xmlf, File xsdf) throws Exception{
		SchemaFactory sf=SchemaFactory.newInstance(XMLConstants.W3C_XML_SCHEMA_NS_URI);
		Schema s =sf.newSchema(new StreamSource(xsdf));	
		Validator validator =s.newValidator();
		validator.validate(new StreamSource(xmlf));
	}
	public static void parseXML(File xmlFile) throws ParserConfigurationException, SAXException, IOException{
		DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
		DocumentBuilder builder = factory.newDocumentBuilder();
		Document doc = builder.parse(xmlFile);		
		doc.getDocumentElement().normalize();
		
        FileWriter fw = new FileWriter("stdout.csv");
        PrintWriter pw = new PrintWriter(fw);
		
        NodeList dayList = doc.getElementsByTagName("gasDay");
        NodeList nList = doc.getElementsByTagName("amountOfPower");

        for (int day=0; day<dayList.getLength(); day++){
        	Node dayNode= dayList.item(day);
        	NamedNodeMap day_nodemap=dayNode.getAttributes();
        	String dayString=day_nodemap.item(0).getNodeValue();
	        for (int temp = 0; temp < nList.getLength(); temp++) {
	        	Node nNode = nList.item(temp);
	        	NamedNodeMap hour_nodemap= nNode.getParentNode().getAttributes();
	            NamedNodeMap power_nodemap= nNode.getAttributes();
	            
	            pw.print(dayString +"; "+hour_nodemap.item(0).getNodeValue()
	               + "; amountOfPower-"+power_nodemap.item(1).getNodeValue()+"\n");
	            
	        }
        }
        pw.flush();
        fw.close();
    }
	

  public static void main(String argv[]) {


	
	//File xmlFile = new File("measured-1.0.0.2017-02-03.b0050c5c8deb1db59c7b2644414b079d.xml");
	//File xmlFile = new File("example.measured-1-1-0.xml");
	File xsdFile = new File("measured-1-1-0.xsd");

	File xmlFile= new File(argv[0]);
	//File xsdFile = new File(argv[1]);


	try{
	validateFile(xmlFile,xsdFile);
	
	}catch(Exception e){
		System.err.println(e.toString());
        try {
        FileWriter fw = new FileWriter("stderr.txt");
		fw.append(e.toString());
        fw.close();
        }        catch (IOException e1) {
			// TODO Auto-generated catch block
        }
        
	}
    try {
    	parseXML(xmlFile);
    }catch(SAXException e){
    	System.err.println("validation failed: "+e.toString());
    }catch(IOException ioe) {
    	System.err.println(ioe.toString());
	}catch( ParserConfigurationException p){
    	System.err.println(p.toString());
	}


    
  
  }
}
