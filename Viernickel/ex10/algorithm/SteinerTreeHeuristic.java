package algorithm;

import datastructure.*;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.LinkedList;
import java.util.Stack;

/**
 * Main algorithm class representing the Dijkstra algorithm.
 * Assumes first entry of the nodes array as starting node.
 * @author Merlin Viernickel
 * @date June 08 2017
 */
public class SteinerTreeHeuristic {

    private Node[] nodes;
    private int[] distances;
    private Node[] predecessors;
    private Edge[] predecessorEdges;
    private boolean[] isTerminal;
    private boolean[] isNodeInSteinerTree;
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
        this.distances = new int[nodes.length];
        this.predecessors = new Node[nodes.length];
        this.predecessorEdges = new Edge[nodes.length];
        this.isTerminal = isTerminal;
        this.isNodeInSteinerTree = new boolean[nodes.length];
        this.visited = new boolean[nodes.length];
        this.treeEdges = new ArrayList<Edge>();
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

        Arrays.fill(this.distances, Integer.MAX_VALUE);
        Arrays.fill(this.predecessors, null);
        Arrays.fill(this.predecessorEdges, null);
        Arrays.fill(this.visited, false);
        Arrays.fill(this.isNodeInSteinerTree, false);

        distances[startTerminal.id] = 0;
        isNodeInSteinerTree[startTerminal.id] = true;
        heap.add(new BinaryHeapNode(currNode.id, distances[currNode.id]));

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
                    isNodeInSteinerTree[currPathNode.id] = true;
                    objectiveValue += predecessorEdges[currPathNode.id].weight;

                    /** Update currPathNode while also resetting distance and predecessor */
                    prevPathNode = currPathNode;
                    currPathNode = predecessors[currPathNode.id];
                    distances[prevPathNode.id] = 0;
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
                newDistance = distances[currNode.id] + currNode.getDistanceToNeighbour(i);
                if(newDistance < distances[neighbourNode.id]){
                    distances[neighbourNode.id] = newDistance;
                    predecessors[neighbourNode.id] = currNode;
                    predecessorEdges[neighbourNode.id] = currNode.edges.get(i);
                    heap.add(new BinaryHeapNode(neighbourNode.id, distances[neighbourNode.id]));
                }
            }
            visited[currNode.id] = true;
        }
        return objectiveValue;
    }

    public boolean buildAndCheckSteinerTree(Node startTerminal){
        LinkedList<Node> queue = new LinkedList<>();
        boolean[] dfsVisited = new boolean[this.nodes.length];
        Node currNode;
        Node neighbourNode;

        queue.add(startTerminal);
        while(!queue.isEmpty()){
            currNode = queue.pop();

            if(dfsVisited[currNode.id])
                return false;
            dfsVisited[currNode.id] = true;

            for(int i=0; i<currNode.edges.size(); i++){
                neighbourNode = currNode.getNeighbour(i);
                if(isNodeInSteinerTree[neighbourNode.id] && currNode.edges.get(i) == predecessorEdges[neighbourNode.id]){
                    assert(predecessorEdges[neighbourNode.id].head == currNode || predecessorEdges[neighbourNode.id].tail == currNode);
                    assert(!dfsVisited[neighbourNode.id]);
                    queue.add(neighbourNode);
                    this.treeEdges.add(predecessorEdges[neighbourNode.id]);
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