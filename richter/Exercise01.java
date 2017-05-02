import java.io.BufferedReader;
import java.io.FileReader;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;

public class Exercise01 {

	public static void main(String[] args) {
			
		long startTime = System.currentTimeMillis();
		
		BufferedReader br = null;
		
		try{
			br = new BufferedReader(new FileReader("ex1-100.dat"));
		}
		catch(Exception e){
			System.out.println("File loading failed.");
		}
			
        String line = "";
        String location;
        double value;
        double sumOfLog1 = 0;
        double sumOfLog2 = 0;
        double geoAveragePos1, geoAveragePos2;
        int validDataPointsPosition1 = 0;
        int validDataPointsPosition2 = 0;
        long progress = 0;
	        
		        
        while (line != null) {
        	
	    	progress++;
	    	
	    	try{
	    		line = br.readLine().replaceAll("\\s+", "");
	    	}
	    	catch(Exception e){
	    		break;
	    	}
	
	    	// ignore line if comment
	    	if(isComment(line))
	    		continue;
	
	        String parts[] = line.split(";");
	    	
	        // ignore line if the given format is not used in the line
	        if(parts.length != 3)
	        	continue;
	        
	    	location = parts[1];
	    	
	    	// ignore line if location is not 1 or 2
	    	if(!(location.equals("1") || location.equals("2")))
	    		continue;
	        
	    	value = Double.parseDouble(parts[2]);
	    	
	    	// ignore line of value is less than zero or NaN
	    	if(value < 0 || Double.isNaN(value))
	    		continue;
	    	
	    	// since we care for the *geometrical* average (product), we can work with logarithms
	    	if(location.equals("1")){
	    		sumOfLog1 += Math.log(value);
	    		validDataPointsPosition1++;
	    	}
	    	
	    	else if(location.equals("2")){
	    		sumOfLog2 += Math.log(value);
	    		validDataPointsPosition2++;
	    	}
    	
        }
		        
		geoAveragePos1 = Math.exp(sumOfLog1 / validDataPointsPosition1);
		geoAveragePos2 = Math.exp(sumOfLog2 / validDataPointsPosition2);
		        
	    System.out.println(progress + " lines");
	    System.out.println("Geometric average position 1: " + geoAveragePos1 + " (" + validDataPointsPosition1 + " valid data points)");
	    System.out.println("Geometric average position 2: " + geoAveragePos2 + " (" + validDataPointsPosition2 + " valid data points)");
	    
	    long elapsedTime = System.currentTimeMillis() - startTime;
		
	    System.out.println("Needed time: " + elapsedTime / 1000 + "sek"); 
}

	private static boolean isComment(String line){
		
		if(line.startsWith("#") || line == null){
			return true;
		}
		
		return false;
		
	}
	
}
