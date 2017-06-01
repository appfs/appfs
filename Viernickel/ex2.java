package ex2;

import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.DocumentBuilder;
import org.w3c.dom.Document;
import org.w3c.dom.NodeList;
import org.w3c.dom.Node;
import org.w3c.dom.Element;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.io.UnsupportedEncodingException;

public class ex2 {

    public static void main(String[] args) {
        Document doc;
        PrintWriter outWriter = null;
        PrintWriter errWriter = null;
        
        NodeList gasDayList;
        NodeList boundaryList;
        NodeList timeList;
        Node gasNode;
        Node boundaryNode;
        Node timeNode;
        Element gasElement;
        Element boundaryElement;
        Element timeElement;
        Element powerElement;
        
        String date;
        byte hour;
        float powerValue;
        
        int i=0;
        int j=0;
        int k=0;
        
        //Set up writers
        try{
            String path = "";
            String[] pathSplit = args[0].split("/");
            for(i=0; i<pathSplit.length-1; i++){
                path = path + pathSplit[i] + "/";
            }
            outWriter = new PrintWriter(path + "stdout", "UTF-8");
            errWriter = new PrintWriter(path + "stderr", "UTF-8");
        } catch (FileNotFoundException e) {
            e.printStackTrace();
            return;
        } catch (UnsupportedEncodingException e) {
            e.printStackTrace();
            return;
        }
        
        //Set up reading of XML file
        try{
            File fXmlFile = new File(args[0]);
            DocumentBuilderFactory dbFactory = DocumentBuilderFactory.newInstance();
            dbFactory.setIgnoringComments(true);
            dbFactory.setIgnoringElementContentWhitespace(true);
            DocumentBuilder dBuilder = dbFactory.newDocumentBuilder();
            doc = dBuilder.parse(fXmlFile);
        } catch (Exception e) {
            e.printStackTrace();
            outWriter.close();
            errWriter.close();
            return;
        }
        
        try {
            gasDayList = doc.getElementsByTagName("gasDay");
            //GasDay loop
            for (i=0; i<gasDayList.getLength(); i++) {
                gasNode = gasDayList.item(i);
                boundaryList = gasNode.getChildNodes();
                
                gasElement = (Element) gasNode;
                date = gasElement.getAttribute("date");

                //Boundary loop
                for(j=0; j<boundaryList.getLength(); j++){
                    boundaryNode = boundaryList.item(j);
                    if(false == boundaryNode.hasAttributes())
                        continue;
                    
                    boundaryElement = (Element) boundaryNode;
                    timeList = boundaryElement.getElementsByTagName("time");
                    
                    //Time loop
                    for(k=0; k<timeList.getLength(); k++){
                        timeNode = timeList.item(k);
                        
                        timeElement = (Element) timeNode;
                        hour = Byte.valueOf(timeElement.getAttribute("hour"));
                        
                        powerElement = (Element) timeElement.getElementsByTagName("amountOfPower").item(0);
                        powerValue = Float.valueOf(powerElement.getAttribute("value"));
                        
                        outWriter.println(date + ";" + hour + ";" + powerValue);
                    }
                    
                }
            }
        } catch (Exception e) {
            errWriter.println(e);
        }
        outWriter.close();
        errWriter.close();
    }
}
