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

    /**
     * Main method
     * @param args String array with path to file as first entry
     */
    public static void main(String[] args){
        long startWallClockTimeNano = System.nanoTime();
        long startUserTimeNano   = getUserTime();
        
        Node[] nodes = Reader.readFile(args[0]);
        
        Dijkstra dijkstra = new Dijkstra(nodes);
        Node furthestNode = dijkstra.findFurthestNode();

        long taskWallClockTimeNano  = System.nanoTime() - startWallClockTimeNano;
        long taskUserTimeNano    = getUserTime( ) - startUserTimeNano;
        
        System.out.println("Wall-clock time elapsed: " + (round(taskWallClockTimeNano/1000000000.0)) + " seconds");
        System.out.println("User time elapsed: " + (round(taskUserTimeNano/1000000000.0)) + " seconds");
        
        System.out.println("RESULT VERTEX " + (furthestNode.id+1));
        System.out.println("RESULT DIST " + furthestNode.distance);
        
        
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
