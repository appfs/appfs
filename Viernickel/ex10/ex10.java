import java.lang.management.ManagementFactory;
import java.lang.management.ThreadMXBean;
import java.util.ArrayList;
import java.util.Arrays;

import datastructure.Edge;
import datastructure.Node;
import datastructure.SteinerThread;
import algorithm.SteinerTreeHeuristic;
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
 * @date July 19 2017
 */
public class ex10 {

    static int nStartTerminals;
    
    /**
     * Main method
     */
    public static void main(String[] args){

        Node[] nodes;
        Node[] terminals;
        boolean[] isTerminal;
        long currentObjectiveValue;
        long lowestObjectiveValue = Long.MAX_VALUE;
        ArrayList<Edge> treeEdges = new ArrayList<>();
        int nTerminals = 0;
        int nThreads = 4;

        /** Read and save nodes and terminals */
        nodes = Reader.readFile(args[0]);
        isTerminal = new boolean[nodes.length];
        for(int i=0; i<nodes.length; i++){
            if(isPrime(i+1)){
                isTerminal[i] = true;
                nTerminals++;
            }
        }

        /** Initialize Wall-clock time and user time */
        long startWallClockTimeNano = System.nanoTime();
        long startUserTimeNano   = getUserTime();

        /** Calculate first n prime numbers and set them as terminals */
        terminals = new Node[nTerminals];
        int termCounter=0;
        for(int i=0; i<nodes.length; i++){
        	if(isTerminal[i]){
        		terminals[termCounter] = nodes[i];
        		termCounter++;
        	}
        }

        /** Calculate Steiner tree and objective value for every starting terminal */
        nStartTerminals = Math.min(Integer.valueOf(args[1]), terminals.length);
        SteinerThread[] threads = new SteinerThread[nThreads];
        SteinerTreeHeuristic[] sths = new SteinerTreeHeuristic[nThreads];
        for(int i=0; i<nStartTerminals; i=i+nThreads){
            for(int j=0; j<nThreads && i+j<nStartTerminals; j++){
                sths[j] = new SteinerTreeHeuristic(nodes, isTerminal);
                threads[j] = new SteinerThread("Thread "+Integer.toString(Integer.valueOf(i)+Integer.valueOf(j)), sths[j], terminals[i+j]);
                threads[j].start();

            }
            while(isRunning(threads))
                ;

            for(int k=0; k<nThreads && i+k<nStartTerminals; k++){
                currentObjectiveValue = sths[k].objectiveValue;
                if(lowestObjectiveValue > currentObjectiveValue){
                    lowestObjectiveValue = currentObjectiveValue;
                    treeEdges = sths[k].treeEdges;
                }
            }
        }

        /** Get Wall-clock time and user time */
        long taskWallClockTimeNano  = System.nanoTime() - startWallClockTimeNano;
        long taskUserTimeNano    = getUserTime( ) - startUserTimeNano;

        /** Get results */
        String fileName = args[0].split("/")[args[0].split("/").length-1];
        String resultValue = "TLEN: " + (lowestObjectiveValue);
        String treeString = "TREE: " + treeEdges.toString().replaceAll("[\\[\\]]", "").replaceAll(", ", " ");
        String userTime = "TIME: " + round(taskUserTimeNano/1000000000.0);
        String wallClockTime = "WALL: " + round(taskWallClockTimeNano/1000000000.0);

        String[] results = {resultValue, treeString, userTime, wallClockTime};
        if(args.length != 3){
             results[1] = results[2];
             results[2] = results[3];
             results = Arrays.copyOf(results, 3);
        }

        /** Print results */
        for(int i=0; i<results.length; i++){
            System.out.println(results[i]);
        }

        /** Save results to file */
        String path = "";
        String[] pathSplit = args[0].split("/");
        for(int i=0; i<pathSplit.length-1; i++){
            path = path + pathSplit[i] + "/";
        }
        path = path + "ex10_results/";
        Writer.write(results, path, fileName);
    }

    private static boolean isRunning(SteinerThread[] threads){
        for(int i=0; i<threads.length && i<nStartTerminals; i++){
            if(threads[i].isAlive())
                return true;
        }
        return false;
    }

    /**
     * Round to three decimal places
     * @param x Value to be rounded
     * @return Rounded value
     */
    public static double round(double x){
        return Math.round(x*1000)/1000.0;
    }

    /** Get the user time in nanoseconds. */
    public static long getUserTime(){
        ThreadMXBean bean = ManagementFactory.getThreadMXBean();
        return bean.isCurrentThreadCpuTimeSupported() ?
            bean.getCurrentThreadUserTime() : 0L;
    }

    /**
     * Check whether a certain number is prime.
     * @param n The number to check
     * @return True if number is prime. False if not.
     */
    public static boolean isPrime(int n){
        assert(n>0);

        if(n<2){
            return false;
        }
        if((n%2 == 0) && (n != 2)){
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