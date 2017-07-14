import java.lang.management.ManagementFactory;
import java.lang.management.ThreadMXBean;

import datastructure.Node;
import algorithm.Dijkstra;
import io.Reader;
import io.Writer;


/** Main class that reads in a file of the specified format and calculates a
 *  Steiner tree using all prime IDs as terminals.
 * Reads a file of the format:
 * nVertices nEdges
 * headId tailId weight
 * headId tailId weight
 *          .
 *          .
 *          .
 * 
 * Prints the objective value of the resulting Steiner tree.
 * 
 * @author Merlin Viernickel
 * @date July 06 2017
 */
public class ex8 {
    
    private static final String MYALGORITHM = "-m1";
    private static final String LIBALGORITHM = "-m2";
    
    /**
     * Main method
     * @param args String array with algorithm mode (-m1 or -m2) as first entry and path to file as second entry
     */
    public static void main(String[] args){
        long startWallClockTimeNano = System.nanoTime();
        long startUserTimeNano   = getUserTime();
        
        Node[] nodes;
        boolean[] isTerminal;
        long objectiveValue;
        
        /** Read and save nodes and terminals */
        nodes = Reader.readFile(args[1]);
        isTerminal = new boolean[nodes.length];
        for(int i=0; i<nodes.length; i++){
            if(isPrime(i+1)){
                isTerminal[i] = true;
            }
        }

        
        /** Calculate Steiner tree and objective value */
        /** Mode 0: Use custom Dijkstra implementation */
        if(args[0].equals(MYALGORITHM)){
            Dijkstra dijkstra = new Dijkstra(nodes);
            objectiveValue = dijkstra.calcSteinerTree(isTerminal, nodes[1]);
        /** Mode 1: Use library Dijkstra implementation */
        }else if(args[0].equals(LIBALGORITHM)){
            objectiveValue = 0;
        }else{
            System.out.println("Invalid Algorithm Mode.");
            System.out.println("Proper usage: ex8 -m1 path/to/file/file.txt");
            return;
        }
        
        /** Get Wall-clock time and user time */
        long taskWallClockTimeNano  = System.nanoTime() - startWallClockTimeNano;
        long taskUserTimeNano    = getUserTime( ) - startUserTimeNano;
        
        /** Get results */
        String fileName = args[1].split("/")[args[1].split("/").length-1];
        String resultValue = "RESULT OBJECTIVE VALUE " + (objectiveValue);
        String wallClockTime = "RESULT WALL-CLOCK TIME " + (round(taskWallClockTimeNano/1000000000.0)) + " seconds";
        String userTime = "RESULT USER TIME " + (round(taskUserTimeNano/1000000000.0)) + " seconds";
        
        String[] results = {fileName, resultValue, wallClockTime, userTime};
        
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
        path = path + "ex8_results/";
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
    
    
    /**
     * Checks whether a certain number is prime.
     * @param n The number to check
     * @return True if number is prime. False if not.
     */
    public static boolean isPrime(int n){
        assert(n>0);
        
        if(n<2){
            return false;
        }
        if(n%2 == 0){
            return false;
        }
        for(int i=3; i*i<=n; i=i+2){
            if(n%i == 0){
                return false;
            }
        }
        return true;
    }
}

