package io;

import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.io.UnsupportedEncodingException;

public class Writer {
    
    public static void writeResults(String[] lines, String path, String fileName){
        PrintWriter writer = null;
        
        /** Set up writers */
        try{
            writer = new PrintWriter(path + fileName, "UTF-8");
        } catch (FileNotFoundException e) {
            e.printStackTrace();
            return;
        } catch (UnsupportedEncodingException e) {
            e.printStackTrace();
            return;
        }
        
        /** Write results to file */
        for(String line : lines){
            writer.println(line);
        }
        
        /** Close writer */
        writer.close();
    }
    
}
