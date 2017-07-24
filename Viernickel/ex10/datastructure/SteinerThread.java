package datastructure;

import java.lang.management.ManagementFactory;
import java.lang.management.ThreadMXBean;

import algorithm.SteinerTreeHeuristic;

/**
 * Wrapper class that runs the Steiner heuristic in a separate thread.
 * @author Merlin Viernickel
 * @date July 22 2017
 */
public class SteinerThread implements Runnable {
    public Thread t;
    private long threadTime;
    private String threadName;
    private SteinerTreeHeuristic sth;
    private Node startTerminal;
    
    /**
     * Constructor
     * @param threadName Name of the thread
     * @param sth Pointer to the Steiner tree heuristic
     * @param startTerminal The starting terminal
     */
    public SteinerThread(String threadName, SteinerTreeHeuristic sth, Node startTerminal) {
       this.threadName = threadName;
       this.sth = sth;
       this.startTerminal = startTerminal;
    }
    
    /**
     * Run function of the runnable object. Calculates a Steiner tree, checks its validity and saves the edges in the sth object
     */
    public void run() {
       sth.calcSteinerTree(startTerminal);
       sth.buildAndCheckSteinerTree(startTerminal);
       this.threadTime = getUserTime();
    }
    
    /**
     * Start function of the runnable object.
     */
    public void start () {
       if (t == null) {
          t = new Thread (this, threadName);
          t.start ();
       }
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
     * Gets the thread time in nanoseconds
     * @return Thread time in nanoseconds
     */
    public long getThreadTime(){
        return this.threadTime;
    }
 }