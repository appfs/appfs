package algorithm;

import datastructure.*;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Stack;

/**
 * Main algorithm class representing the Dijkstra algorithm.
 * Assumes first entry of the nodes array as starting node.
 * @author Merlin Viernickel
 * @date June 08 2017
 */
public class SteinerTreeHeuristic {
    
    private Node[] nodes;
    private boolean[] isTerminal;
    private boolean[] isNodeInSteinerTree;
    private boolean[] touched;
    private boolean[] visited;
    public ArrayList<Edge> treeEdges;
    
    /**
     * Constructor
     * @param nodes Array of nodes to calculate distance from
     */
    public SteinerTreeHeuristic(Node[] nodes, boolean[] isTerminal){
        assert(null != nodes);
        assert(null != isTerminal);
        assert(1 <= nodes.length);
        assert(nodes.length == isTerminal.length);
        
        this.nodes = nodes;
        this.isTerminal = isTerminal;
        this.treeEdges = new ArrayList<Edge>();
        this.isNodeInSteinerTree = new boolean[nodes.length];
        this.touched = new boolean[nodes.length];
        this.visited = new boolean[nodes.length];
    }
    
    /**
     * Calculates a Steiner tree using the Dijkstra algorithm heuristic method.
     */
    public long calcSteinerTree(Node startTerminal){
        Node currNode = startTerminal;
        Node neighbourNode = null;
        Node currPathNode;
        Node prevPathNode;
        BinaryHeap<BinaryHeapNode> heap = new BinaryHeap<BinaryHeapNode>();
        BinaryHeapNode lowestHeapNode;
        long objectiveValue = 0;
        int neighbourId = 0;
        int newDistance;
        
        Arrays.fill(this.touched, false);
        Arrays.fill(this.visited, false);
        Arrays.fill(this.isNodeInSteinerTree, false);
        
        startTerminal.distance = 0;
        isNodeInSteinerTree[startTerminal.id] = true;
        heap.add(new BinaryHeapNode(currNode));
        
        while(!heap.isEmpty()){
            /** Update currNode */
            lowestHeapNode = heap.remove();
            if(visited[lowestHeapNode.id])
                continue;
            currNode = nodes[lowestHeapNode.id];
            
            /** If we found a terminal, add its path to the Steiner tree */
            if(isTerminal[currNode.id]){
                currPathNode = currNode;
                
                /** Find shortest path from predecessors and save nodes in Steiner tree */
                while(!isNodeInSteinerTree[currPathNode.id]){
                    objectiveValue += currPathNode.distance;
                    isNodeInSteinerTree[currPathNode.id] = true;
                    
                    /** Update currPathNode while also resetting distance and predecessor */
                    prevPathNode = currPathNode;
                    currPathNode = currPathNode.predecessor;
                    prevPathNode.distance = 0;
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
                
                /** Update distance and add to heap */
                newDistance = currNode.distance + currNode.getDistanceToNeighbour(i);
                if(newDistance < neighbourNode.distance){
                    neighbourNode.distance = newDistance;
                    neighbourNode.predecessor = currNode;
                    neighbourNode.predecessorEdge = currNode.edges.get(i);
                    heap.add(new BinaryHeapNode(neighbourNode));
                }
                
                touched[neighbourId] = true;
            }
            visited[currNode.id] = true;
        }
        return objectiveValue;
    }
    
    public boolean buildAndCheckSteinerTree(Node startTerminal){
        Stack<Node> stack = new Stack<>();
        boolean[] dfsVisited = new boolean[this.nodes.length];
        Node currNode;
        Node neighbourNode;
        
        stack.push(startTerminal);
        while(!stack.isEmpty()){
            currNode = stack.pop();
            
            if(dfsVisited[currNode.id])
                return false;
            dfsVisited[currNode.id] = true;
            
            for(int i=0; i<currNode.edges.size(); i++){
                neighbourNode = currNode.getNeighbour(i);
                if(isNodeInSteinerTree[neighbourNode.id] && (currNode == neighbourNode.predecessor)){
                    stack.push(neighbourNode);
                    this.treeEdges.add(neighbourNode.predecessorEdge);
                }
            }
        }
        
        for(int i=0; i<this.nodes.length; i++){
            if(isTerminal[i] && !dfsVisited[i])
                return false;
        }
        return true;
    }
}