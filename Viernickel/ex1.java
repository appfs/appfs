package ex1;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;

public class ex1{
    
    public static void main(String[] args){
        FileReader  fr    = null;
        BufferedReader br;
        double[]     logSum  = new double[2];
        int[]       size    = new int[2];
        double[]    geoMean = new double[2];
        byte[]  loc;
        float[] val;
        byte    tmp_loc;
        float   tmp_val;

        int i = 0;

        try {
            fr = new FileReader(args[0]);
            br = new BufferedReader(fr);
            /* Count lines for memory allocation */
            for(i=0; br.readLine() != null; i++) {}
            br.close();
            fr = new FileReader(args[0]);
            br = new BufferedReader(fr);

            String line;
            loc = new byte[i];
            val = new float[i];
            String noCommentLine;
            String[] parts;

            /* Get all entries in memory */
            for(i=0; (line = br.readLine()) != null; i++) {
                try {
                    line.trim();
                    noCommentLine = line.split("#")[0];
                    parts = noCommentLine.split(";");
                    Integer.valueOf(parts[0].trim());
                    tmp_loc = Byte.valueOf(parts[1].trim());
                    tmp_val = Float.valueOf(parts[2].trim());
                } catch (Exception e) {
                    //System.out.println("Line ignored: " + line);
                    continue;
                }
                if( (Float.isNaN(tmp_val)) || (2 < tmp_loc) || (0 > tmp_val) || (1 > tmp_loc)){
                    //System.out.println("Line ignored: " + line);
                    continue;
                }
                loc[i] = tmp_loc;
                val[i] = tmp_val;
            }
            br.close();

            for(i=0; i < val.length; i++) {
                if (loc[i] > 0) {
                    logSum[loc[i] - 1] += Math.log(val[i]);
                    size[loc[i] - 1] += 1;
                }
            }

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
