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

		explicit GraphParser(std::string file);

		bool readFirstLine(int& numVertices, int& numEdges);

		/**
		 * Reads in a graph from a file in the .gph format. and stores all information
		 * in the other parameters
		 * @param ref to a stream of the file to be read
		 * @param numVertices ref to where the number of vertices should be stored
		 * @param numEdges ref to where the number of edges should be stored
		 * @param edges array in which the edges should be stored
		 * @param weights array in which the weights should be stored
		 * @return true, if file was read successfully, false otherwise
		 */
		bool read(Edge edges[], double weights[]);

		bool readSuccessfully;

	private:
		std::ifstream inputFile;


};




#endif /* SRC_GRAPHPARSER_H_ */
