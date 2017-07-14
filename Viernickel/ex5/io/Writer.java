package io;

import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.io.UnsupportedEncodingException;

/**
 * Output class containing methods to write results to files
 * @author Merlin Viernickel
 * @date July 05 2017
 */
public class Writer {
    
    /**
     * Writes given lines to a file of specified path
     * @param lines Array of Strings containing the lines to be written
     * @param path Path to the file
     * @param fileName Name of the file
     */
    public static void write(String[] lines, String path, String fileName){
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
