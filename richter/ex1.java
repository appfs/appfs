import java.io.BufferedReader;
import java.io.FileReader;
import java.util.ArrayList;

public class ex1 {

	static float[] pos1 = new float[51000000];
	static float[] pos2 = new float[51000000];
	
	public static void main(String[] args) {
			
		long startTime = System.currentTimeMillis();
		
		BufferedReader br = null;
		
		try{
			br = new BufferedReader(new FileReader(args[0]));
		}
		catch(Exception e){
			System.out.println("File loading failed.");
		}
			
        String line = "";
        int seqnum;
        int location;
        Float value;
        double sumOfLog1 = 0;
        double sumOfLog2 = 0;
        double geoAveragePos1, geoAveragePos2;
        int validDataPointsPosition1 = 0;
        int validDataPointsPosition2 = 0;
        long progress = -1;
	        
		        
        while (line != null) {
        	
	    	progress++;

	    	try{
	    		
	    		line = br.readLine();
	    		
	    	}
	    	catch(Exception e){
	    		
	    		break;
	    		
	    	}
	    	
	    	if(line == null)
	    		continue;
	
	    	
	    	try{
	    		
		    	// removes characters after a #
	    		line = line.split("#")[0];
	    		
	    		String parts[] = line.split(";");
	    		seqnum = Integer.valueOf(parts[0].trim());
		    	location = Integer.valueOf(parts[1].trim());
		    	value = Float.valueOf(parts[2].trim());
		    	
		    	// ignore line if location is not 1 or 2
		    	if(!( location == 1 || location == 2)){
//		    		System.err.println("Invalid location format.");
		    		continue;
		    	}

		    	
		    	// ignore line of value is less than zero or NaN
		    	if(value < 0 || Double.isNaN(value)){
//		    		System.err.println("Invalid number format.");
		    		continue;
		    	}
		    	
		    	// since we care for the *geometrical* average (product), we can work with logarithms
		    	if(location == 1){
//		    		sumOfLog1 += Math.log(value);
//		    		pos1.add(value);
		    		pos1[validDataPointsPosition1] = value;
		    		validDataPointsPosition1++;
		    	}
		    	
		    	else if(location == 2){
//		    		sumOfLog2 += Math.log(value);
//		    		pos2.add(value);
		    		pos2[validDataPointsPosition2] = value;
		    		validDataPointsPosition2++;
		    	}
	    		
	    	}
	    	
	    	catch(Exception e){

	    		continue;
	    		
	    	}
    	
        }
		        
        for(int i = 0; i < validDataPointsPosition1; i++){
        	
        	sumOfLog1 += Math.log(pos1[i]);
        	
        }
        
        for(int i = 0; i < validDataPointsPosition2; i++){
        	
        	sumOfLog2 += Math.log(pos2[i]);
        	
        }
        
		geoAveragePos1 = Math.exp(sumOfLog1 / validDataPointsPosition1);
		geoAveragePos2 = Math.exp(sumOfLog2 / validDataPointsPosition2);
		        
	    System.out.println(progress + " lines");
	    System.out.println("Geometric average position 1: " + geoAveragePos1 + " (" + validDataPointsPosition1 + " valid data points)");
	    System.out.println("Geometric average position 2: " + geoAveragePos2 + " (" + validDataPointsPosition2 + " valid data points)");
	    
	    long elapsedTime = System.currentTimeMillis() - startTime;
		
	    System.out.println("Needed time: " + elapsedTime / 1000 + "sek"); 
	}
	
}
