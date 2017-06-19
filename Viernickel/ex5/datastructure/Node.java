package datastructure;

import java.util.ArrayList;

/**
 * Datastructure representing a node in a graph
 * @author Merlin Viernickel
 * @date June 08 2017
 */
public class Node implements Comparable<Node>{
    
    public int id;
    public int distance;
    public boolean visited;
    public boolean touched;
    public ArrayList<Neighbour> neighbours;
    
    /**
     * Constructor
     * @param id Id of constructed node
     */
    public Node(int id){
        this.id = id;
        this.distance = Integer.MAX_VALUE;
        this.visited = false;
        this.touched = false;
        this.neighbours = new ArrayList<Neighbour>();
    }

    /**
     * Compares nodes based on their distance
     * Function used by priority queue
     */
    @Override
    public int compareTo(Node otherNode) {
        return Integer.compare(this.distance, otherNode.distance);
    }
    
    
}

