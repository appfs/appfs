package datastructure;

import java.lang.management.ManagementFactory;
import java.lang.management.ThreadMXBean;

import algorithm.SteinerTreeHeuristic;

public class SteinerThread implements Runnable {
    public Thread t;
    private long threadTime;
    private String threadName;
    private SteinerTreeHeuristic sth;
    private Node startTerminal;
    
    public SteinerThread(String threadName, SteinerTreeHeuristic sth, Node startTerminal) {
       this.threadName = threadName;
       this.sth = sth;
       this.startTerminal = startTerminal;
    }
    
    public void run() {
       sth.calcSteinerTree(startTerminal);
       sth.buildAndCheckSteinerTree(startTerminal);
       this.threadTime = getUserTime();
    }
    
    public void start () {
       if (t == null) {
          t = new Thread (this, threadName);
          t.start ();
       }
    }
    
    /** Get the user time in nanoseconds. */
    public static long getUserTime(){
        ThreadMXBean bean = ManagementFactory.getThreadMXBean();
        return bean.isCurrentThreadCpuTimeSupported() ?
            bean.getCurrentThreadUserTime() : 0L;
    }
    
    public long getThreadTime(){
        return this.threadTime;
    }
    
    public boolean isAlive(){
        return t.isAlive();
    }
 }