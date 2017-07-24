import java.lang.management.ManagementFactory;
import java.lang.management.ThreadMXBean;
import java.util.ArrayList;
import java.util.Arrays;

import datastructure.Edge;
import datastructure.Node;
import datastructure.SteinerThread;
import algorithm.SteinerTreeHeuristic;
import io.Reader;


/** Main class that reads in a file of the specified format and calculates a
 *  Steiner tree using all prime IDs as terminals.
 *  
 * Reads a file of the format:
 * nVertices nEdges
 * headId tailId weight
 * headId tailId weight
 *          .
 *          .
 *          .
 *
 * Prints the objective value of the resulting Steiner tree, CPU and wall-clock time needed for the calculation
 * and optionally the edges of the Steiner tree.
 *
 * @author Merlin Viernickel
 * @date July 19 2017
 */
public class ex10 {

    static final int DEFAULT_THREAD_NUMBER = 4;
    static int nThreads = DEFAULT_THREAD_NUMBER;
    static int nStartTerminals;
    
    /**
     * Main method
     * @param args ex10 file.gph n_start_points (-s) (nThreads)
     */
    public static void main(String[] args){
        Node[] nodes;
        Node[] terminals;
        SteinerThread[] threads;
        SteinerTreeHeuristic[] sths;
        ArrayList<Edge> treeEdges = new ArrayList<>();
        boolean[] isTerminal;
        boolean printTree = false;
        int nTerminals = 0;
        long startWallClockTimeNano;
        long startUserTimeNano;
        long threadTimeNano;
        long currentObjectiveValue;
        long lowestObjectiveValue;
        String resultValue;
        String treeString;
        String userTime;
        String wallClockTime;


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
        startWallClockTimeNano = System.nanoTime();
        startUserTimeNano = getUserTime();
        threadTimeNano = 0;

        /** Calculate first n prime numbers and set them as terminals */
        terminals = new Node[nTerminals];
        int termCounter=0;
        for(int i=0; i<nodes.length; i++){
        	if(isTerminal[i]){
        		terminals[termCounter] = nodes[i];
        		termCounter++;
        	}
        }

        nStartTerminals = Math.min(Integer.valueOf(args[1]), terminals.length);
        /** Interpret arguments for number of threads and whether or not to print the Steiner tree edges */
        if(args.length > 2){
            if(args[2].equals("-s")){
                printTree = true;
                if(args.length == 4){
                    nThreads = Integer.valueOf(args[3]);
                }
            }else{
                nThreads = Integer.valueOf(args[2]);
            }
        }
        
        /** Run the heuristic from every starting terminal and keep the lowest objective value */
        lowestObjectiveValue = Long.MAX_VALUE;
        threads = new SteinerThread[nThreads];
        sths = new SteinerTreeHeuristic[nThreads];
        /** Loop over starting terminals */
        for(int i=0; i<nStartTerminals; i=i+nThreads){
            /** Create nThreads new threads and run the heuristic from a new terminal in each one of them */
            for(int j=0; j<nThreads && i+j<nStartTerminals; j++){
                sths[j] = new SteinerTreeHeuristic(nodes, isTerminal);
                threads[j] = new SteinerThread("Thread "+Integer.toString(Integer.valueOf(i)+Integer.valueOf(j)), sths[j], terminals[i+j]);
                threads[j].start();
            }

            /** Join all threads with this one */
            for(int j=0; j<nThreads && i+j<nStartTerminals; j++){
                try {
                    threads[j].t.join();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }

            /** After termination, update objective value and treeEdges pointer if new optimum was found */
            for(int j=0; j<nThreads && i+j<nStartTerminals; j++){
                threadTimeNano += threads[j].getThreadTime();
                currentObjectiveValue = sths[j].objectiveValue;
                if(lowestObjectiveValue > currentObjectiveValue){
                    lowestObjectiveValue = currentObjectiveValue;
                    treeEdges = sths[j].treeEdges;
                }
            }
        }

        /** Get Wall-clock time and user time */
        long taskWallClockTimeNano  = System.nanoTime() - startWallClockTimeNano;
        long taskUserTimeNano    = getUserTime( ) - startUserTimeNano;

        /** Get results */
        resultValue = "TLEN: " + (lowestObjectiveValue);
        treeString = "TREE: " + treeEdges.toString().replaceAll("[\\[\\]]", "").replaceAll(", ", " ");
        userTime = "TIME: " + round((taskUserTimeNano+threadTimeNano)/1000000000.0);
        wallClockTime = "WALL: " + round(taskWallClockTimeNano/1000000000.0);

        String[] results = {resultValue, treeString, userTime, wallClockTime};
        if(!printTree){
             results[1] = results[2];
             results[2] = results[3];
             results = Arrays.copyOf(results, 3);
        }

        /** Print results */
        for(int i=0; i<results.length; i++){
            System.out.println(results[i]);
        }
    }

    /**
     * Round to three decimal places
     * @param x Value to be rounded
     * @return Rounded value
     */
    public static double round(double x){
        return Math.round(x*1000)/1000.0;
    }

    /**
     * Gets the user time in nanoseconds
     * @return User time in nanoseconds
     */
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