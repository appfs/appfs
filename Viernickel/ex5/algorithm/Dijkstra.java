package algorithm;

import datastructure.*;

import java.util.ArrayList;
import java.util.PriorityQueue;
import java.util.Stack;

/**
 * Main algorithm class representing the Dijkstra algorithm.
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
     * Dijkstra's algorithm that calculates the distance for all nodes from the starting node
     */    
    public void calculateDistances(Node startNode){
        Node currNode = startNode;
        Node neighbourNode = null;
        int neighbourId = 0;
        int newDistance;
        boolean changed = false;
        
        startNode.distance = 0;
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
                    neighbourNode.predecessor = currNode;
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
                changed = false;
            }
            visited[currNode.id] = true;
            
            /** Update currNode */
            currNode = queue.poll();
        }while(null != currNode);
        
    }
    
    /**
     * Calculates a Steiner tree using the Dijkstra algorithm heuristic method.
     */
    public long calcSteinerTree(boolean[] isTerminal, Node startTerminal){
        assert(isTerminal.length == this.nodes.length);
        
        Node currNode = startTerminal;
        Node neighbourNode = null;
        ArrayList<Node> tree = new ArrayList<Node>();
        Node[] pathToTree;
        long objectiveValue = 0;
        int neighbourId = 0;
        int newDistance;
        boolean changed = false;
        
        startTerminal.distance = 0;
        queue.add(currNode);
        
        do{
            /** If we found a terminal, add its path to the Steiner tree */
            if(isTerminal[currNode.id]){
                pathToTree = getShortestPath(currNode);
                for(int i=0; i<pathToTree.length; i++){
                    objectiveValue += pathToTree[i].distance;
                    pathToTree[i].distance = 0;
                    pathToTree[i].predecessor = null;
                    tree.add(pathToTree[i]);
                }
            }
            /** Touch all neighboring nodes */
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
                    neighbourNode.predecessor = currNode;
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
                changed = false;
            }
            visited[currNode.id] = true;
            
            /** Update currNode */
            currNode = queue.poll();
        }while(null != currNode);
        return objectiveValue;
    }

    
    
    /**
     * Gets the highest distance node with the lowest id
     * @return Highest distance node with lowest id
     */
    public Node getHighestDistNode(){
        
        assert(this.nodes.length > 1);
        
        Node highestDistNode = this.nodes[0];
        for(int i=1; i<this.nodes.length; i++){
            if(highestDistNode.distance < nodes[i].distance)
                highestDistNode = nodes[i];
        }
        return highestDistNode;
    }
    
    /**
     * Gets the shortest path from the starting node to the destination node.
     * @param destinationNode Destination node of the shortest path
     * @return Array of nodes contained in the shortest path
     */
    public Node[] getShortestPath(Node destinationNode){
        Stack<Node> stack = new Stack<>();
        Node currNode = destinationNode;
        
        assert(destinationNode.predecessor != null);
        
        /** Find shortest path from predecessors */
        stack.push(destinationNode);
        while(currNode.predecessor != null){
            stack.push(currNode.predecessor);
            currNode = currNode.predecessor;
        }
        
        /** Save shortest path in array */
        Node[] shortestPath = new Node[stack.size()];
        for(int i=0; i<shortestPath.length; i++){
            shortestPath[i] = stack.pop();
        }
        
        return shortestPath;
    }
}
