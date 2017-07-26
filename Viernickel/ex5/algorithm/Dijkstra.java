package algorithm;

import datastructure.*;

import java.util.ArrayList;
import java.util.Stack;

/**
 * Main algorithm class representing the Dijkstra algorithm.
 * Assumes first entry of the nodes array as starting node.
 * @author Merlin Viernickel
 * @date June 08 2017
 */
public class Dijkstra {

    private Node[] nodes;
    public int[] distances;
    private boolean[] touched;
    private boolean[] visited;
    private BinaryHeap<BinaryHeapNode> queue;
    public ArrayList<Edge> treeEdges;

    /**
     * Constructor
     * @param nodes Array of nodes to calculate distance from
     */
    public Dijkstra(Node[] nodes){
        assert(null != nodes);
        assert(1 <= nodes.length);

        this.nodes = nodes;
        this.distances = new int[nodes.length];
        this.touched = new boolean[nodes.length];
        this.visited = new boolean[nodes.length];
        this.queue = new BinaryHeap<BinaryHeapNode>();
        this.treeEdges = new ArrayList<Edge>();
    }

    /**
     * Dijkstra's algorithm that calculates the distance for all nodes from the starting node
     */
    public void calculateDistances(Node startNode){
        Node currNode = startNode;
        Node neighbourNode = null;
        BinaryHeapNode lowestHeapNode;
        int neighbourId = 0;
        int newDistance;

        queue.add(new BinaryHeapNode(currNode.id, distances[currNode.id]));

        do{
            for(int i=0; i<currNode.edges.size(); i++){
                neighbourNode = currNode.getNeighbour(i);
                neighbourId = neighbourNode.id;

                /** Skip already visited nodes */
                if(visited[neighbourId]){
                    continue;
                }

                /** Update distance */
                newDistance = distances[currNode.id] + currNode.getDistanceToNeighbour(i);
                if(newDistance < distances[neighbourNode.id]){
                    distances[neighbourNode.id] = newDistance;
                    queue.add(new BinaryHeapNode(neighbourNode.id, distances[neighbourNode.id]));
                }


                touched[neighbourId] = true;
            }
            visited[currNode.id] = true;

            /** Update currNode */
            lowestHeapNode = queue.remove();
            if(lowestHeapNode != null)
                currNode = nodes[lowestHeapNode.id];
        }while(null != lowestHeapNode);
    }
        /**
     * Gets the highest distance node with the lowest id
     * @return Highest distance node with lowest id
     */
    public int getHighestDistanceNode(){

        assert(this.nodes.length > 1);

        Node highestDistNode = this.nodes[0];
        for(int i=1; i<this.nodes.length; i++){
            if(distances[highestDistNode.id] < distances[nodes[i].id])
                highestDistNode = nodes[i];
        }
        return highestDistNode.id;
    }}
