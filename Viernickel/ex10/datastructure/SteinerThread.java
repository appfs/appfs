package datastructure;

import algorithm.SteinerTreeHeuristic;

public class SteinerThread implements Runnable {
    private Thread t;
    private String threadName;
    private SteinerTreeHeuristic sth;
    private Node startTerminal;
    
    public SteinerThread(String threadName, SteinerTreeHeuristic sth, Node startTerminal) {
       this.threadName = threadName;
       this.sth = sth;
       this.startTerminal = startTerminal;
       System.out.println("Creating " +  threadName );
    }
    
    public void run() {
       System.out.println("Running " +  threadName );
       
       sth.calcSteinerTree(startTerminal);
       sth.buildAndCheckSteinerTree(startTerminal);
       
       System.out.println("Thread " +  threadName + " exiting.");
    }
    
    public boolean isAlive(){
        return t.isAlive();
    }
    
    public void start () {
       System.out.println("Starting " +  threadName );
       if (t == null) {
          t = new Thread (this, threadName);
          t.start ();
       }
    }
 }