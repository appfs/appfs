package datastructure;

/**
 * Class representing binary heap node elements with distance as key and id as data.
 * @author Merlin Viernickel
 * @date July 22 2017
 */
public class BinaryHeapNode implements Comparable<BinaryHeapNode> {

	public int id;
	public int distance;

	/**
	 * Constructor
	 * @param id Node id
	 * @param distance Node distance
	 */
	public BinaryHeapNode(int id, int distance){
		this.id = id;
		this.distance = distance;
	}

    /**
     * Compares this node to another node based on their distance.
     * @param otherNode Node to compare this node to
     */
    @Override
    public int compareTo(BinaryHeapNode otherNode) {
        return Integer.compare(this.distance, otherNode.distance);
    }

}
