import datastructure.*;
import dijkstra.*;
import io.*;
import java.lang.management.*;

/** @brief Main class that reads in a file of the specified format and calculates the
 * furthest vertex from vertex 1 and its distance.
 * 
 * Reads a file of the format:
 * nVertices nEdges
 * headId tailId weight
 * headId tailId weight
 *          .
 *          .
 *          .
 * 
 * Prints the result vertex and restult distance
 * @author Merlin Viernickel
 * @date June 08 2017
 */
public class ex5 {
    
    private static final String MYALGORITHM = "0";
    private static final String LIBALGORITHM = "1";
    
    /**
     * Main method
     * @param args String array with algorithm mode (0 or 1) as first entry and path to file as second entry
     */
    public static void main(String[] args){
        long startWallClockTimeNano = System.nanoTime();
        long startUserTimeNano   = getUserTime();
        
        Node[] nodes = Reader.readFile(args[1]);
        Node furthestNode = null;
        
        /** Mode 0: Use custom Dijkstra implementation */
        if(args[0].equals(MYALGORITHM))
        {
            Dijkstra dijkstra = new Dijkstra(nodes);
            furthestNode = dijkstra.findFurthestNode();                   
        /** Mode 1: Use library Dijkstra implementation */
        }else if(args[0].equals(LIBALGORITHM)){
        	JGraphTDijkstra dijkstra = new JGraphTDijkstra(nodes);
        	furthestNode = dijkstra.findFurthestNode();
        }else{
            System.out.println("Invalid Algorithm Mode.");
            System.out.println("Proper usage: ex5 0 path/to/file/file.txt");
            return;
        }
        
        /** Get Wall-clock time and user time */
        long taskWallClockTimeNano  = System.nanoTime() - startWallClockTimeNano;
        long taskUserTimeNano    = getUserTime( ) - startUserTimeNano;
        
        /** Print results */
        
        System.out.println("RESULT VERTEX " + (furthestNode.id+1));
        System.out.println("RESULT DIST " + furthestNode.distance);
        
        System.out.println("RESULT WALL-CLOCK TIME: " + (round(taskWallClockTimeNano/1000000000.0)) + " seconds");
        System.out.println("RESULT USER TIME: " + (round(taskUserTimeNano/1000000000.0)) + " seconds");
    }
    
    /** Round to three decimal places */
    public static double round(double x){
        return Math.round(x*1000)/1000.0;
    }
     
    /** Get user time in nanoseconds. */
    public static long getUserTime(){
        ThreadMXBean bean = ManagementFactory.getThreadMXBean();
        return bean.isCurrentThreadCpuTimeSupported() ?
            bean.getCurrentThreadUserTime() : 0L;
    }
}
