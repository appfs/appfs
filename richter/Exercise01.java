import java.io.BufferedReader;
import java.io.FileReader;

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
	        double valueLocation1 = 0;
	        double valueLocation2 = 0;
	        long validDataPointsPosition1 = 0;
	        long validDataPointsPosition2 = 0;
	        long progress = 0;
	        double meanPosition1, meanPosition2;
	        
	        double highestValue = 0;

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
	        	
	        	//ignore line of value is less than zero, more than 1000 or NaN
	        	if(value < 0 || value > 1000 || Double.isNaN(value))
	        		continue;
	        	
	        	if(location.equals("1")){
	        		valueLocation1 += value;
	        		validDataPointsPosition1++;
	        	}
	        	
	        	else if(location.equals("2")){
	        		valueLocation2 += value;
	        		validDataPointsPosition2++;
	        	}
	        	
	        }

	        meanPosition1 = valueLocation1 / (double)validDataPointsPosition1;
	        meanPosition2 = valueLocation2 / (double)validDataPointsPosition2;

	        System.out.println(progress + " lines");
	        System.out.println("Mean position 1: " + meanPosition1 + " (" + validDataPointsPosition1 + " valid data points)");
	        System.out.println("Mean position 2: " + meanPosition2 + " (" + validDataPointsPosition2 + " valid data points)");
	        
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
