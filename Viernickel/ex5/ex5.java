import datastructure.*;
import dijkstra.*;
import io.*;

/** @brief Main class that reads in a file of the specified format and calculates the
 * furthest vertex from vertex 1 and its distance.
 * 
 * Reads a file of the format:
 * nVertices nEdges
 * headId tailId weight
 * headId tailId weight
 * .
 * .
 * .
 * 
 * Prints the result vertex and restult distance
 * @author Merlin Viernickel
 * @date June 08 2017
 */
public class ex5 {

    /**
     * Main method
     * @param args String array with path to file as first entry
     */
    public static void main(String[] args){
        Node[] nodes = Reader.readFile(args[0]);
        
        Dijkstra dijkstra = new Dijkstra(nodes);
        Node furthestNode = dijkstra.findFurthestNode();
        
        System.out.println("RESULT VERTEX " + (furthestNode.id+1));
        System.out.println("RESULT DIST " + furthestNode.distance);
    }
    
}
