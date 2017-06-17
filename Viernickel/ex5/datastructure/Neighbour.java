package datastructure;

/**
 * Datastructure class representing the edges as neighbours of Nodes
 * @author merlin
 * @date June 08 2017
 */
public class Neighbour {
    
    public int id;
    public int weight;
    
    /**
     * Constructor
     * @param id Id of the neighbour node
     * @param weight Weight of the edge to neighbour node
     */
    public Neighbour(int id, int weight){
        this.id = id;
        this.weight = weight;
    }
    
}

