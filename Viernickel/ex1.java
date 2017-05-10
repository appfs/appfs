package ex1;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;

public class ex1{
    
    public static void main(String[] args){
        FileReader  fr    = null;
        double[]    logSum  = new double[2];
        int[]       size    = new int[2];
        double[]    geoMean = new double[2];
        int seq;
        int loc;
        double val;
        int i = 0;
        
        try {
            fr = new FileReader(args[0]);
            BufferedReader br = new BufferedReader(fr);
            String line;
            for(i=0; (line = br.readLine()) != null; i++){
                try {
                    line.trim();
                    String noCommentLine = line.split("#")[0];
                    String[] parts = noCommentLine.split(";");
                    seq = Integer.valueOf(parts[0].trim());
                    loc = Integer.valueOf(parts[1].trim());
                    val = Double.valueOf(parts[2].trim());
                } catch (Exception e) {
                    //System.out.println("Line ignored: " + line);
                    continue;
                }
                if( (Double.isNaN(val)) || (2 < loc) || (0 > val) || (1 > loc)){
                    //System.out.println("Line ignored: " + line);
                    continue;
                }
                logSum[loc-1] += Math.log(val);
                size[loc-1]   += 1;
            }
            br.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
        geoMean[0] = Math.exp(logSum[0] / size[0]); 
        geoMean[1] = Math.exp(logSum[1] / size[1]);
        String[] pathSplit = args[0].split("/");
        String filename = pathSplit[pathSplit.length-1];

        System.out.println("File: " + filename + " with " + Integer.toString(i) + " lines");
        System.out.println("Valid values Loc1: " + Integer.toString(size[0]) + " with GeoMean: " + Double.toString(geoMean[0]));
        System.out.println("Valid values Loc2: " + Integer.toString(size[1]) + " with GeoMean: " + Double.toString(geoMean[1]));
    }
}
