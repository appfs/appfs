/**
 * @file 	GraphParser.h
 * @author 	Fabian Wegscheider
 * @date 	Jul 10, 2017
 */


#ifndef SRC_GRAPHPARSER_H_
#define SRC_GRAPHPARSER_H_

#include <fstream>

using Edge = std::pair<int,int>;


class GraphParser {
	public:

		/**
		 * Constructs a parser for .gph files.
		 * @param file path to the file which should be read
		 */
		explicit GraphParser(std::string file);

		/**
		 * Reads the first line of a .gph file and stores the data about the graph
		 * @param numVertices ref to where the number of vertices should be stored
		 * @param numEdges ref to where the number of edges should be stored
		 * @return whether the line was read successfully
		 */
		bool readFirstLine(int& numVertices, int& numEdges);

		/**
		 * Reads in the edges of a graph from a file in the .gph format and
		 * stores all information in the other parameters. It expects that the
		 * first line is read already
		 * @param edges array in which the edges should be stored
		 * @param weights array in which the weights should be stored
		 * @return whether the file was read successfully, false otherwise
		 */
		bool readEdgeData(Edge edges[], double weights[]);


		/**
		 * Indicates whether the file could be found and opened
		 */
		bool openedSuccessfully;

	private:
		std::ifstream inputFile;


};




#endif /* SRC_GRAPHPARSER_H_ */
