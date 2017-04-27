import java.io.*;
import java.lang.*;


public class Ex1 {

	public static void main(String[]args){

		long start = System.currentTimeMillis(); 
		double[] averages = new double[2];
		long[] index = new long[2]; 
		int group; 
		Double val;

		System.out.println("Reading file and calculating means..."); 

	    File file = new File("ex1-100.dat");

	    try{
	    	BufferedReader br = new BufferedReader(new FileReader(file));
	        String line;
	        while ((line = br.readLine()) != null) {
	           if(line.length() == 0) continue;
	           else if(line.charAt(0) == '#') continue;
	           else{
	          		String[] parts = line.split("; ");
	          		if(parts.length == 3){
	     				if(!(parts[1].equals("1") || parts[1].equals("2")))continue;
	     				if(parts[2].contains(" "))continue;
	          			group = Integer.parseInt(parts[1]);
	          			val = Double.parseDouble(parts[2]);

	          			if(val.isNaN())continue; 

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
	    System.out.println("Finished in " + String.valueOf(time) + " seconds."); 
	    System.out.println("Valid values Loc1: " + String.valueOf(index[0]) + " with GeoMean: " + String.valueOf(averages[0])); 
	    System.out.println("Valid values Loc2: " + String.valueOf(index[1]) + " with GeoMean: " + String.valueOf(averages[1])); 

	}
}