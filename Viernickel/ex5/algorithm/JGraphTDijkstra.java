package algorithm;

import org.jgrapht.ListenableGraph;
import org.jgrapht.alg.DijkstraShortestPath;
import org.jgrapht.graph.GraphDelegator;
import org.jgrapht.graph.ListenableUndirectedWeightedGraph;
import org.jgrapht.graph.DefaultWeightedEdge;

import datastructure.Edge;
import datastructure.Node;

/**
 * Wrapper class for the library Dijkstra algorithm implementation
 * @author Merlin Viernickel
 * @date 24.06.2017
 */
public class JGraphTDijkstra {
	
	private Node[] nodes;
	
	/**
	 * Constructor
	 * @param nodes Array of nodes to calculate distance from
	 */
	public JGraphTDijkstra(Node[] nodes){
		this.nodes = nodes;
	}
	
	/**
     * Dijkstra algorithm that calculates the distance for all nodes and returns the furthest one
     * @return Node furthest from the starting node
	 */
	public Node findFurthestNode(Node startNode){
        ListenableGraph<Node, DefaultWeightedEdge> graph = new ListenableUndirectedWeightedGraph<Node, DefaultWeightedEdge>(DefaultWeightedEdge.class);
        Node furthestNode = nodes[0];
        
        /** Add vertices */
        for(int i=0; i<nodes.length; i++){
            graph.addVertex(nodes[i]);
        }
        
        /** Add edges */
        for(int i=0; i<nodes.length; i++){
            Node node = nodes[i];
            for(int j=0; j<node.edges.size(); j++){
                if(node.edges.get(j).head.equals(node)){
                	DefaultWeightedEdge edge = new DefaultWeightedEdge();
                    graph.addEdge(node, node.edges.get(j).tail, edge);
                    ((GraphDelegator<Node, DefaultWeightedEdge>) graph).setEdgeWeight(edge, (double) node.edges.get(j).weight);
                }
            }
        }
        
        /** Calculate distances and find furthest Node */
        for(int i=1; i<nodes.length; i++){
            DijkstraShortestPath<Node, DefaultWeightedEdge> shortestPath =
            		new DijkstraShortestPath<Node, DefaultWeightedEdge>(graph, startNode, nodes[i]);
            
            nodes[i].distance = (int) shortestPath.getPathLength();
            if(furthestNode.distance < nodes[i].distance){
            	furthestNode = nodes[i];
            }
        }
        
        return furthestNode;
	}

}
