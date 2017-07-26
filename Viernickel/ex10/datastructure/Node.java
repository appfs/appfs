package datastructure;

import java.util.ArrayList;

/**
 * Datastructure representing a node in a graph
 * @author Merlin Viernickel
 * @date June 08 2017
 */
public class Node {

    public int id;
    public ArrayList<Edge> edges;

    /**
     * Constructor
     * @param id Id of constructed node
     */
    public Node(int id){
        this.id = id;
        this.edges = new ArrayList<Edge>();
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

    /**
     * String representation of the object.
     */
    public String toString(){
        return Integer.toString(this.id);
    }
}

