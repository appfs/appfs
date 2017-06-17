/**
 * 2012 ADM I - TU Berlin
 * 
 * A class for simple Digraphs used in the NetworkSimplexAlgorithm
 * Needs interaction with classes GraphNode and GraphEdge
 * 
 * @author Jan and Mona
 */



import java.util.ArrayList;
import java.util.Iterator;


public class Graph implements Iterable<GraphNode> {
	
	/** The array of nodes */
	protected ArrayList<GraphNode> nodes;
	
	/** The number of nodes in the graph */
	private int numOfNodes = 0;
	
	/** The number of edges in the graph */
	private int numOfEdges = 0;
	
	
	/**
	 * Creates a new graph with given number of nodes
	 * All nodes are initialized with flow balance zero.
	 * @param size the number of nodes
	 */
	public Graph(int size) {
		nodes = new ArrayList<GraphNode>(size);
		//Initialize nodes
		for (int i=0;i<size;i++)
			nodes.add(i,new GraphNode(i));
		numOfNodes = size;
	}
	

	
	/**
	 * Adds an edge between two given nodes. 
	 * If an edge between these nodes already exists an exception is thrown.
	 * 
	 * @param start the id of the start node
	 * @param end the id of the end node
	 * @param low the lower flow capacity
	 * @param cap the upper flow capacity
	 * @param cost the costs of this edge
	 * @param flow the initial flow on this edge
	 * @return the newly created edge
	 * @throws IllegalArgumentException if the edge already exists
	 */
	public GraphEdge addEdge(int start, int end, int cost) throws IllegalArgumentException {
		if (edgeExists(start,end)){
			throw new IllegalArgumentException("Edge already exists. ("+start+", "+end+")");
		}
		

		GraphEdge edge = new GraphEdge(nodes.get(start),nodes.get(end),cost);		
		nodes.get(start).addEdge(edge);
		nodes.get(end).addEdge(edge);
		
		numOfEdges++;
		
		return edge;
	}
	
	/**
	 * Returns the edge between two nodes, <code>null</code> if no such edge exists.
	 * @param i start node
	 * @param j end node
	 * @return edge
	 */
	public GraphEdge getEdge(int i, int j){
		GraphNode node = nodes.get(i);
		for (int k=0; k<node.getEdgeCount();k++){
			if (node.getNeighbour(k).id()==nodes.get(j).id()){
				return node.getEdge(k);
			}
		}
		return null;
	}
	
	/**
	 * Checks, if edge already exists
	 * @param i start node
	 * @param j end node
	 * @return true, if edge exists, false otherwise
	 */
	public boolean edgeExists(int i, int j){
		if ((!(nodeExists(i))) || (!(nodeExists(j)))){
			return false;
		}
		
		GraphNode node = nodes.get(i);
		for (int k=0; k<node.getEdgeCount();k++){
			if (node.getNeighbour(k)==nodes.get(j)){
				return true;
			}
		}
		return false;
	}
	
	/**
	 * Checks, if a node exists. 
	 * @param i id of node
	 * @return true if node exists, false otherwise
	 */
	public boolean nodeExists(int i) {
		try{
			if (nodes.get(i)!=null){
				return true;
			}
		}catch(Exception e){}
		return false;
	}
	

	
	/**
	 * Returns the node with the given index.
	 * @param i the index
	 * @return  the node
	 */
	public GraphNode getNode(int i) {
		return nodes.get(i);
	}

	/**
	 * Returns the number of edges in the graph.
	 * @return  the number of edges
	 */
	public int getNumOfEdges() {
		return numOfEdges;
	}

	/**
	 * Returns the number of nodes in the graph. 
	 * @return  the number of nodes
	 */
	public int getNumOfNodes() {
		return numOfNodes;
	}
	
	/**
	 * Returns a node iterator that iterates over all nodes.
	 * @return a node iterator
	 */
	public Iterator<GraphNode> iterator() {
		return nodes.iterator();
	}
	

	
	/**
	 * A simple String representation of the graph
	 */
	public String toString() {
		return "Graph with " + numOfNodes + " nodes and " + numOfEdges + " edges.";
	}
}