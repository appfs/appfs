import java.lang.management.ManagementFactory;
import java.lang.management.ThreadMXBean;
import java.util.ArrayList;

import datastructure.Node;
import algorithm.*;
import io.Reader;


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
        Node[] terminals;
        Integer[] primes;
        int objectiveValue;
        
        /** Read and save nodes and terminals */
        nodes = Reader.readFile(args[1]);
        primes = getPrimesUpToN(nodes.length+1);
        terminals = new Node[primes.length];
        for(int i=0; i<primes.length; i++){
            terminals[i] = nodes[primes[i]-1];
        }
        
        /** Calculate Steiner tree and objective value */
        /** Mode 0: Use custom Dijkstra implementation */
        if(args[0].equals(MYALGORITHM)){
            SteinerTreeHeuristic sth = new SteinerTreeHeuristic(nodes, terminals);
            sth.calcSteinerTree();
            objectiveValue = sth.objectiveValue;
        /** Mode 1: Use library Dijkstra implementation */
        }else if(args[0].equals(LIBALGORITHM)){
            objectiveValue = 0;
        }else{
            System.out.println("Invalid Algorithm Mode.");
            System.out.println("Proper usage: ex5 -m1 path/to/file/file.txt");
            return;
        }
        
        /** Get Wall-clock time and user time */
        long taskWallClockTimeNano  = System.nanoTime() - startWallClockTimeNano;
        long taskUserTimeNano    = getUserTime( ) - startUserTimeNano;
        
        /** Print results */
        System.out.println("RESULT OBJECTIVE VALUE: " + objectiveValue);
        
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
    
    /**
     * Calculates all primes up to a given number and returns them in an array.
     * @param n The number up to which primes are calculated
     * @return Array of prime numbers
     */
    public static Integer[] getPrimesUpToN(int n){
        assert(n>0);
        
        Integer[] primeArray;
        ArrayList<Integer> primes = new ArrayList<Integer>();
        
        for(int i=0; i<n; i++){
            if(isPrime(i)){
                primes.add(i);
            }
        }
        primeArray = new Integer[primes.size()];
        for(int i=0; i< primeArray.length; i++){
            primeArray[i] = primes.get(i);
        }
        return primeArray;
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

