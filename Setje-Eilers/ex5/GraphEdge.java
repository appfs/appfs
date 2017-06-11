/**
 * 2012 ADM I - TU Berlin
 * 
 * A class for simple Digraph-Edges used in the NetworkSimplexAlgorithm
 * Needs interaction with classes GraphNode and Graph
 * 
 * @author Mona
 */


/**
 * An edge to be used in a {@link Graph}. The edge has references of the
 * start and end nodes and a some flow related data fields.
 */
public class GraphEdge {
	
	
	/** the cost of this edge */
	private int cost;
		
	
	/** The start node. */
	protected GraphNode start;
	
	/** The end node. */
	protected GraphNode end;
					
	public GraphEdge(GraphNode start, GraphNode end, int cost) {
		this.start=start;
		this.end=end;
		this.cost = cost;

	}


	
	/**
	 * Returns the cost of the edge.
	 * @return the cost of the edge
	 */
	public int getCost(){
		return cost;
	}
	

	
	/**
	 * Sets the cost of the edge.
	 * @param data the cost of the edge
	 */
	public void setCost(int cost){
		this.cost=cost;
	}
	
	/**
	 * Returns the start node of the edge
	 * @return the start node
	 */
	public GraphNode getStart() {
		return start;
	}
	

	/**
	 * Returns the end node of the edge
	 * @return the end node
	 */
	public GraphNode getEnd() {
		return end;
	}
	
	
	/**
	 * Checks if two edges are equal. 
	 * They are equal, if they have the same start
	 * and end nodes and the data is the same
	 * @param obj the object checked for equality
	 * @return {@code true} or {@code false}
	 */
	public boolean equals(Object obj) {
		if(obj == null)
			return false;
		if(getClass() != obj.getClass())
			return false;
		final GraphEdge other = (GraphEdge)obj;
		if(this.start != other.start)
			return false;
		if(this.end != other.end)
			return false;
		if(!(other.cost==cost))
			return false;
		return true;
	}

	/**
	 * String representation of this edge.
	 */
	public String toString(){
		return "edge<"+getStart().id()+","+getEnd().id();
	}
}
