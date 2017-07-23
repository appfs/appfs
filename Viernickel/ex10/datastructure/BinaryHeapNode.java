package datastructure;

public class BinaryHeapNode implements Comparable<BinaryHeapNode> {

	public int id;
	public int distance;

	public BinaryHeapNode(int id, int distance){
		this.id = id;
		this.distance = distance;
	}

    /**
     * Compares nodes based on their distance
     * Function used by priority queue
     */
    @Override
    public int compareTo(BinaryHeapNode otherNode) {
        return Integer.compare(this.distance, otherNode.distance);
    }

}
