package io;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.util.Scanner;

import datastructure.*;

/**
 * Reads a file of the format:
 * nVertices nEdges
 * headId tailId weight
 * headId tailId weight
 * .
 * .
 * .
 * 
 * Saves nodes in an array and edges as neighbours within the Node datastructure
 * @author Merlin Viernickel
 * @date June 08 2017
 */
public class Reader{
    
    /**
     * Reads the file returns a Node array
     * @param path Path to file
     * @return Array of nodes read
     */
    public static Node[] readFile(String path){
        FileReader fr = null;
        BufferedReader br;
        String line;
        Scanner scanner;
        String[] split;
        int nNodes;
        int nEdges;
        Node[] nodes = null;
        int headId;
        int tailId;
        int weight;
        int i;
        int j;
        
        
        try {
            //fr = new FileReader(path);
            //br = new BufferedReader(fr);
            scanner = new Scanner(new File(path));
            
            /** Read first line */
            scanner.nextInt();
            //line = br.readLine();
            nNodes = scanner.nextInt();
            nEdges = scanner.nextInt();
            
            /** Initialize node array */
            nodes = new Node[nNodes];
            for(i=0; i<nNodes; i++){
                nodes[i] = new Node(i);
            }
            
            /** Read and save edges */
            for(j=0; j<nEdges; j++){
                //line = br.readLine();
                //split = line.split(" ");
                headId = scanner.nextInt();
                tailId = scanner.nextInt();
                weight = scanner.nextInt();
                
                //Edge edge = new Edge(nodes[headId], nodes[tailId], weight);
                
                //nodes[headId].edges.add(edge);
                //nodes[tailId].edges.add(edge);
                System.out.println(headId + " " + tailId + " " + weight);
            }
            System.out.println("finished reading");
            //br.close();
            } catch (IOException e) {
            e.printStackTrace();
        }
        return nodes;
    }
}