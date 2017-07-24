package algorithm;

import datastructure.*;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.LinkedList;

/**
 * Algorithm class representing the Steiner tree heuristic.
 * @author Merlin Viernickel
 * @date June 23 2017
 */
public class SteinerTreeHeuristic {

    private Node[] nodes;
    private Node[] predecessors;
    private Edge[] predecessorEdges;
    private int[] distances;
    private boolean[] isTerminal;
    private boolean[] isNodeInSteinerTree;
    private boolean[] visited;
    public ArrayList<Edge> treeEdges;
    public long objectiveValue;
    public boolean isFeasible = true;

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
     * @param startTerminal The starting terminal
     */
    public void calcSteinerTree(Node startTerminal){
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
        this.objectiveValue = objectiveValue;
    }

    /**
     * Checks the Steiner tree for validity and saves the edges in treeEdges
     * @param startTerminal
     */
    public void buildAndCheckSteinerTree(Node startTerminal){
        LinkedList<Node> queue = new LinkedList<>();
        boolean[] bfsVisited = new boolean[this.nodes.length];
        Node currNode;
        Node neighbourNode;

        /** Runs through the tree using breadth first search, only using Steiner tree edges */
        queue.add(startTerminal);
        while(!queue.isEmpty()){
            currNode = queue.pop();

            /** If we find a cycle it is not a tree and therefore not valid */
            if(bfsVisited[currNode.id]){
                this.isFeasible = false;
                return;
            }
            bfsVisited[currNode.id] = true;

            /** Throw all adjacent nodes that are connected by Steiner tree edges into the queue */
            for(int i=0; i<currNode.edges.size(); i++){
                neighbourNode = currNode.getNeighbour(i);
                if(isNodeInSteinerTree[neighbourNode.id] && currNode.edges.get(i) == predecessorEdges[neighbourNode.id]){
                    assert(predecessorEdges[neighbourNode.id].head == currNode || predecessorEdges[neighbourNode.id].tail == currNode);
                    assert(!bfsVisited[neighbourNode.id]);
                    queue.add(neighbourNode);
                    this.treeEdges.add(predecessorEdges[neighbourNode.id]);
                }
            }
        }

        /** Check whether all terminals were reached */
        for(int i=0; i<this.nodes.length; i++){
            if(isTerminal[i] && !bfsVisited[i]){
                this.isFeasible = false;
                return;
            }
        }
    }
}