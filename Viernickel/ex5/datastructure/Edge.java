package datastructure;

/**
 * Datastructure class representing the edges in the graph
 * @author Merlin Viernickel
 * @date June 08 2017
 */
public class Edge {
    
    public Node head;
    public Node tail;
    public int weight;
    

    /**
     * Constructor method
     * @param head Head of the edge
     * @param tail Tail of the edge
     * @param weight Weight of the edge
     */
    public Edge(Node head, Node tail, int weight){
        this.head = head;
        this.tail = tail;
        this.weight = weight;
    }
    
}