import java.io.*;
import java.lang.*;


public class Ex1 {

	public static void main(String[]args){

		String filename = args[0];

		long start = System.currentTimeMillis(); 
		double[] averages = new double[2];
		long[] index = new long[2]; 
		int group; 
		Double val;
		int lines = 0; 

		String seqMatcher = "[0-9]+";
		String groupMatcher = "[1-2]";
		String floatMatcher = "[0-9.e-]+";

		//System.out.println("Reading file and calculating means..."); 

	    File file = new File(filename);

	    try{
	    	BufferedReader br = new BufferedReader(new FileReader(file));
	        String line;
	        while ((line = br.readLine()) != null) {
	        	lines ++;

	        	if( 0 == line.length()) {
	           		continue;
	           	}

	           	if('#' == line.charAt(0)) {
	           		continue;
	           	}
	           	else{
	           		line = (line.split("#"))[0];
	          		String[] parts = line.split(";");
	          		if(parts.length == 3){
	          			parts[0] = parts[0].trim();
	          			parts[1] = parts[1].trim();
	          			parts[2] = parts[2].trim();

	     				if (!(parts[0].matches(seqMatcher) && parts[1].matches(groupMatcher) && parts[2].matches(floatMatcher))){
	          				logError(line, lines, "Format");
	          				continue; 
	          			}

	          			group = Integer.parseInt(parts[1]);
	          			val = Double.parseDouble(parts[2]);
	          			val = Math.log(val);
	          			averages[group-1] = (averages[group-1]*index[group-1] + val)/(index[group-1]+1);
	          			
	          			//System.out.println("Valid values Loc1: " + String.valueOf(index[0]) + " with GeoMean: " + String.valueOf(averages[0])); 
	    				//System.out.println("Valid values Loc2: " + String.valueOf(index[1]) + " with GeoMean: " + String.valueOf(averages[1])); 

	          			index[group-1]++; 
	          		}
	          		else{
	          			logError(line, lines, "Sections");
						continue;
					}
	           }
	        }
	    }catch(Exception e){
	    	System.out.println("An error occured");
	    	System.out.println(e.toString());
	    }

	    long end = System.currentTimeMillis(); 
	    long time = (end -start)/1000; 
	    //System.out.println("Finished in " + String.valueOf(time) + " seconds."); 
	    System.out.println("File: " + String.valueOf(filename) + " with " + String.valueOf(lines) +" lines"); 
	    System.out.println("Valid values Loc1: " + String.valueOf(index[0]) + " with GeoMean: " + String.valueOf(Math.exp(averages[0]))); 
	    System.out.println("Valid values Loc2: " + String.valueOf(index[1]) + " with GeoMean: " + String.valueOf(Math.exp(averages[1]))); 
	}
	public static void logError(String line, int lineNumber, String errKey){
		System.out.println("Error in line " + String.valueOf(lineNumber) + " (" + errKey + "): " + line); 
	}
}