package algorithm;

import java.util.ArrayList;

import datastructure.Node;

public class SteinerTreeHeuristic {
    
    private Node[] nodes;
    private Node[] terminals;
    private ArrayList<Node> tree;
    public int objectiveValue;
    
    public SteinerTreeHeuristic(Node[] nodes, Node[] terminals){
        this.nodes = nodes;
        this.terminals = terminals;
        this.tree = new ArrayList<Node>();
        this.objectiveValue = 0;
    }
    
    public void calcSteinerTree(){
        Node dockingNode;
        Node[] shortestPath;
        
        /** Calculate the Steiner tree and its objective value */
        tree.add(terminals[0]);
        for(int i=1; i<terminals.length; i++){
            /** If terminal already in tree, continue */
            if(tree.contains(terminals[i])){
                continue;
            }
            /** Connect new terminal to current tree and update objective value */
            Dijkstra dijkstra = new Dijkstra(nodes);
            dijkstra.calculateDistances(terminals[i]);
            dockingNode = dijkstra.getLowestDistNodeToSteinerTree(tree);
            this.objectiveValue = this.objectiveValue + dockingNode.distance;
            shortestPath = dijkstra.getShortestPath(dockingNode);
            for(int j=0; j<shortestPath.length; j++){
                if(!tree.contains(shortestPath[j])){
                    tree.add(shortestPath[j]);
                }
            }
            resetNodes();
        }
    }
    
    private void resetNodes(){
        for(int i=0; i<nodes.length; i++){
            nodes[i].distance = Integer.MAX_VALUE;
            nodes[i].predecessor = null;
        }
    }
    
}
