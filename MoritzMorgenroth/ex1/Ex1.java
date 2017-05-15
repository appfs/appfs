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

		String groupMatcher = "[0-1]";
		String floatMatcher = "[0-9.]+";

		//System.out.println("Reading file and calculating means..."); 

	    File file = new File(filename);

	    try{
	    	BufferedReader br = new BufferedReader(new FileReader(file));
	        String line;
	        while ((line = br.readLine()) != null) {
	        	lines ++;
	           if( 0 == line.length()) continue;
	           if('#' == line.charAt(0)) continue;
	           else{
	           		line = (line.split("#"))[0];
	           		line = line.replace(" ", "");
	          		String[] parts = line.split(";");
	          		if(parts.length == 3){
	     				if (!(parts[1].matches(groupMatcher) && parts[2].matches(floatMatcher)))continue;

	          			group = Integer.parseInt(parts[1]);
	          			val = Double.parseDouble(parts[2]);

	          			if(val.isNaN())continue; 

	          			val = Math.log(val);
	          			averages[group-1] = (averages[group-1]*index[group-1] + val)/(index[group-1]+1);
	          			
	          			//System.out.println("Valid values Loc1: " + String.valueOf(index[0]) + " with GeoMean: " + String.valueOf(averages[0])); 
	    				//System.out.println("Valid values Loc2: " + String.valueOf(index[1]) + " with GeoMean: " + String.valueOf(averages[1])); 

	          			index[group-1]++; 
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
}