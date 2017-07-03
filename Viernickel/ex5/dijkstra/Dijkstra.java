package dijkstra;

import datastructure.*;
import java.util.PriorityQueue;

/**
 * Main algorithm class representing te Dijkstra algorithm.
 * Assumes first entry of the nodes array as starting node.
 * @author Merlin Viernickel
 * @date June 08 2017
 */
public class Dijkstra {
    
    private Node[] nodes;
    private boolean[] touched;
    private boolean[] visited;
    private PriorityQueue<Node> queue;
    
    /**
     * Constructor
     * @param nodes Array of nodes to calculate distance from
     */
    public Dijkstra(Node[] nodes){
        assert(null != nodes);
        assert(1 <= nodes.length);
        this.nodes = nodes;
        this.touched = new boolean[nodes.length];
        this.visited = new boolean[nodes.length];
        this.queue = new PriorityQueue<Node>();
    }
    
    /**
     * Dijkstra algorithm that calculates the distance for all nodes and returns the furthest one
     * @return Node furthest from the starting node
     */
    public Node findFurthestNode(){
        Node currNode = nodes[0];
        Node neighbourNode = null;
        int neighbourId = 0;
        int newDistance;
        boolean changed = false;
        
        queue.add(currNode);
        
        do{
            for(int i=0; i<currNode.edges.size(); i++){
                neighbourNode = currNode.getNeighbour(i);
                neighbourId = neighbourNode.id;
                
                /** Skip already visited nodes */
                if(visited[neighbourId]){
                    continue;
                }
                
                /** Update distance */
                newDistance = currNode.distance + currNode.getDistanceToNeighbour(i);
                if(newDistance < neighbourNode.distance){
                    neighbourNode.distance = newDistance;
                    changed = true;
                }
                
                /** Update priorityQueue */
                if(touched[neighbourId] && changed){
                    queue.remove(neighbourNode);
                    queue.add(neighbourNode);
                }else if(changed){
                    queue.add(neighbourNode);
                }
                
                touched[neighbourId] = true;
            }
            visited[currNode.id] = true;
            
            /** Update currNode */
            currNode = queue.poll();
        }while(null != currNode);
        
        return this.getHighestDistNode();
    }
    
    /**
     * Gets the highest distance node with the lowest id
     * @return Highest distance node with lowest id
     */
    private Node getHighestDistNode(){
        assert(this.nodes.length > 1);
        Node highestDistNode = this.nodes[1];
        for(int i=2; i<this.nodes.length; i++){
            if(highestDistNode.distance < nodes[i].distance)
                highestDistNode = nodes[i];
        }
        return highestDistNode;
    }
}
