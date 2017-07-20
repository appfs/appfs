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
    public Node predecessor;
    public Edge predecessorEdge;
    public ArrayList<Edge> edges;
    
    /**
     * Constructor
     * @param id Id of constructed node
     */
    public Node(int id){
        this.id = id;
        this.distance = Integer.MAX_VALUE;
        this.predecessor = null;
        this.edges = new ArrayList<Edge>();
    }

    /**
     * Compares nodes based on their distance
     * Function used by priority queue
     */
    @Override
    public int compareTo(Node otherNode) {
        return Integer.compare(this.distance, otherNode.distance);
    }
    
    /**
     * Returns the i-th neighbour node
     * @param i i-th neighbour
     * @return i-th neighbour node
     */
    public Node getNeighbour(int i){
        Edge edge = this.edges.get(i);
        if(this == edge.head)
            return edge.tail;
        else
            return edge.head;
    }
    
    /**
     * Get the distance to the i-th neighbour node
     * @param i i-th neighbour
     * @return distance to the i-th neighbour node
     */
    public int getDistanceToNeighbour(int i){
        return this.edges.get(i).weight;
    }
    
    public String toString(){
        return Integer.toString(this.id);
    }
}

