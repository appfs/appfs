import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.PriorityQueue;

/**
 * Advanced Programming: Exercise 5 (calculating longest shortest path)
 * @author Maximilian Richter
 */

public class ex5 {
	
	/**
	 * @param Please provide the file name containing the graph as String.
	 */
	public static void main(String[] args) {
	
		Node[] nodeList = null;
		
		try {
			
			nodeList = readNodeList(args[0]);
			
		} catch (IOException e) {
			
			System.err.println("Parsing graph failed.");
			
		}
		
		int currentMax = 0;
		int nodeNumber = 0;
		
		ArrayList<Node> resultNodes = shortestPath(nodeList);
		
		for(Node node : resultNodes){
			int distance = node.currentDistance;
			if(distance == Integer.MAX_VALUE)
				continue;
			if(currentMax < distance || (currentMax == distance && node.nodeNumber < nodeNumber)){
				currentMax = distance;
				nodeNumber = node.nodeNumber;
			}
		}
		
		System.out.println("RESULT VERTEX " + nodeNumber);
		System.out.println("RESULT DIST " + currentMax);
	
	}
	
	/**
	 * @param fileName Name of the file to be parsed.
	 * @return A list of nodes which represent the graph, each node contains all its edges as attribute.
	 * @throws IOException This exception is thrown if the given file name does not match an existing file on the system.
	 */
	static Node[] readNodeList(String fileName) throws IOException{
		
		BufferedReader br = null;

		br = new BufferedReader(new FileReader(fileName));
			
        String line = br.readLine();
        int numberOfNodes = Integer.parseInt(line.split(" ")[0]);
        Node[] nodeList = new Node[numberOfNodes];
        
        for(int i = 0; i < numberOfNodes; i++){
        	
        	nodeList[i] = new Node(i + 1, new ArrayList<AdjacentNode>());
        	
        }
        
        int node1, node2, distance;
        while (line != null) {

        	try{
		    	line = br.readLine();
		    	String parts[] = line.split(" ");
		    	node1 = Integer.parseInt(parts[0]);
		    	node2 = Integer.parseInt(parts[1]);
		    	distance = Integer.parseInt(parts[2]);
	
		    	nodeList[node1 - 1].adjacentNodes.add(new AdjacentNode(node2, distance));
		    	nodeList[node2 - 1].adjacentNodes.add(new AdjacentNode(node1, distance));
        	}
        	catch(Exception e){
        		continue;
        	}
	    	
        }
		
        br.close();
        
        return nodeList;
        
	}
	
	
	/**
	 * @param graph Given a graph as a node list this function computes the shortest paths from the first node (start node) to all other nodes.
	 * @return A list of nodes is returned, each node contains its distance to the start node as attribute.
	 */
	static ArrayList<Node> shortestPath(Node[] graph){
		
		ArrayList<Node> resultNodes = new ArrayList<Node>();
		Node currentNode = null;
		PriorityQueue<Node> unvisitedNodes = new PriorityQueue<Node>(new NodeComparator());
		for(Node node : graph){
			unvisitedNodes.add(node);
		}
		
		currentNode = unvisitedNodes.poll();
			
		resultNodes.add(currentNode);
		
		while(currentNode.currentDistance != Integer.MAX_VALUE){
			
			for(AdjacentNode node : currentNode.adjacentNodes){
				Node adjNode = find(unvisitedNodes, node.nodeNumber);
				unvisitedNodes.remove(adjNode);
				if(adjNode == null){
					continue;
				}
				adjNode.currentDistance = Math.min(adjNode.currentDistance, currentNode.currentDistance + node.distance);
				unvisitedNodes.add(adjNode);
			}
			
			currentNode = unvisitedNodes.poll();
			if(currentNode == null)
				break;
			resultNodes.add(currentNode);
		}
		
		return resultNodes;
      
	}
	
	
	static Node find(PriorityQueue<Node> queue, int nodeNumber){
				
		for (Node node : queue){
			if(node.nodeNumber == nodeNumber)
				return node;
		}
		
		return null;

	}
	
}	


/**
 * Definition of a Node. A node holds its node number, its adjacent nodes and a current distance, needed for the shortest path algorithm.
 */
class Node{
	
	int nodeNumber;
	ArrayList<AdjacentNode> adjacentNodes;
	int currentDistance;
	
	Node(int nodeNumber, ArrayList<AdjacentNode> adjacentNodes){
		
		this.nodeNumber = nodeNumber;
		this.adjacentNodes = adjacentNodes;
		currentDistance = Integer.MAX_VALUE;
		if(nodeNumber == 1)
			currentDistance = 0;
		
	}
	
}
	
/**
 * Definition of an adjacent node, used for the shortest path algorithm.
 */
class AdjacentNode{
	
	int nodeNumber;
	int distance;
	
	AdjacentNode(int nodeNumber, int distance){
		
		this.nodeNumber = nodeNumber;
		this.distance = distance;
		
	}
	
}

/**
 * Technical comparator to compare nodes via their current distance to the start node.
 */
class NodeComparator implements Comparator<Node>{
	
 	@Override
 	public int compare(Node a, Node b)
    {
	 	return a.currentDistance - b.currentDistance;
    }
	 
}

