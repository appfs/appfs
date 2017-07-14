/**
 * 2012 ADM I - TU Berlin
 * 
 * A class for simple Digraph-Nodes used in the NetworkSimplexAlgorithm
 * Needs interaction with classes Graph and GraphEdge
 * 
 * @author Mona
 */


import java.util.ArrayList;
import java.util.Iterator;

/**
 * A node to be used in a {@link Graph}. The node has a list of adjacent
 * nodes (incoming and outgoing) and data field for its flow balance.
 */
public class GraphNode implements Iterable<GraphEdge> {
		
	/** The id of this node. */
	private int id;
	
	
	
	/** The list of edges. */
	protected ArrayList<GraphEdge> edges = new ArrayList<GraphEdge>();

	
	
	
	/**
	 * Creates a new instance of a node and stores some data.
	 * 
	 * @param id the nodes index
	 * @param balance the nodes flow balance
	 */
	public GraphNode(int id) {
		this.id=id;

	}
	

	
	/**
	 * Adds an edge to the node.
	 * @param edge the new edge
	 */
	void addEdge(GraphEdge edge) {
		edges.add(edge);
	}
	

	
	/**
	 * Returns the id of the node.
	 * @return the id of the node
	 */
	public int id() {
		return id;
	}



	/**
	 * Returns an iterator that iterates over all outgoing edges.
	 * @return an iterator that iterates over all outgoing edges
	 */
	public Iterator<GraphEdge> iterator() {
		return edges.iterator();
	}

	/**
	 * Returns the number of outgoing (incident) edges.
	 * @return the number of outgoing (incident) edges
	 */
	public int getEdgeCount() {
		return edges.size();
	}
	
	/**
	 * Returns the adjacent node via a specific outgoing edge.
	 * @param i the number of the edge
	 * @return the adjacent node via the {@code i}th outgoing edge
	 */
	public GraphNode getNeighbour(int i) {
		return edges.get(i).getEnd();
	}
	
	/**
	 * Returns a specific outgoing edge.
	 * @param i the number of the edge
	 * @return the {@code i}th outgoing edge
	 */
	public GraphEdge getEdge (int i){
		return edges.get(i);
	}
	


	/**
	 * Check if two nodes are equal.
	 * Two nodes equal if they have the same id and the same data.
	 * @param obj the object to compare with
	 * @return <code>true</code> if the two nodes are equal. <code>false</code> otherwise
	 */
	public boolean equals(Object obj) {
		if(obj == null)
			return false;
		if(getClass() != obj.getClass())
			return false;
		final GraphNode other = (GraphNode)obj;
		return other.id == this.id;
	}
	
	/**
	 * Returns a string representation of the node containing its id and its
	 * data.
	 * @return a string representation of the node
	 */
	public String toString() {
		return id + ": " + edges.toString();
	}
	


}
