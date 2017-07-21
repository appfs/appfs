import datastructure.*;
import algorithm.Dijkstra;
import algorithm.JGraphTDijkstra;
import io.*;
import java.lang.management.*;

/** Main class that reads in a file of the specified format and calculates the
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
    
    private static final String MYALGORITHM = "-m1";
    private static final String LIBALGORITHM = "-m2";
    
    /**
     * Main method
     * @param args String array with algorithm mode (-m1 or -m2) as first entry and path to file as second entry
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
            dijkstra.calculateDistances(nodes[0]);
            furthestNode = dijkstra.getHighestDistNode();
        /** Mode 1: Use library Dijkstra implementation */
        }else if(args[0].equals(LIBALGORITHM)){
        	JGraphTDijkstra dijkstra = new JGraphTDijkstra(nodes);
        	furthestNode = dijkstra.findFurthestNode(nodes[0]);
        }else{
            System.out.println("Invalid Algorithm Mode.");
            System.out.println("Proper usage: ex5 -m1 path/to/file/file.txt");
            return;
        }
        
        /** Get Wall-clock time and user time */
        long taskWallClockTimeNano  = System.nanoTime() - startWallClockTimeNano;
        long taskUserTimeNano    = getUserTime( ) - startUserTimeNano;
        
        /** Get results */
        String fileName = args[1].split("/")[args[1].split("/").length-1];
        String resultVertex = "RESULT VERTEX " + (furthestNode.id+1);
        String resultDist = "RESULT DIST " + furthestNode.distance;
        String wallClockTime = "RESULT WALL-CLOCK TIME " + (round(taskWallClockTimeNano/1000000000.0)) + " seconds";
        String userTime = "RESULT USER TIME " + (round(taskUserTimeNano/1000000000.0)) + " seconds";
        
        String[] results = {fileName, resultVertex, resultDist, wallClockTime, userTime};
        
        /** Print results */
        for(int i=0; i<results.length; i++){
            System.out.println(results[i]);
        }
        
        /** Save results to file */
        String path = "";
        String[] pathSplit = args[1].split("/");
        for(int i=0; i<pathSplit.length-1; i++){
            path = path + pathSplit[i] + "/";
        }
        path = path + "ex5_results/";
        Writer.write(results, path, fileName);
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
