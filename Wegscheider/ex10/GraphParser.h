/**
 * @file 	GraphParser.h
 * @author 	Fabian Wegscheider
 * @date 	Jul 10, 2017
 */


#ifndef SRC_GRAPHPARSER_H_
#define SRC_GRAPHPARSER_H_

#include <fstream>

using Edge = std::pair<int,int>;
using weight_type = double;


class GraphParser {
	public:

		/**
		 * Constructs a parser for .gph files.
		 * @param file path to the file which should be read
		 */
		explicit GraphParser(std::string file);

		/**
		 * Reads the first line of a .gph file and stores the data about the graph
		 * @param num_vertices ref to where the number of vertices is stored
		 * @param num_edges ref to where the number of edges is stored
		 * @return whether the line was read successfully
		 */
		bool read_first_line(int& num_vertices, int& num_edges);

		/**
		 * Reads in the edges of a graph from a file in the .gph format and
		 * stores all information in the other parameters. It expects that the
		 * first line is read already
		 * @param edges array in which the edges are stored
		 * @param weights array in which the weights are stored
		 * @return whether the file was read successfully, false otherwise
		 */
		bool read_edge_data(Edge edges[], weight_type weights[]);


		/**
		 * Indicates whether the file could be found and opened
		 */
		bool opened_successfully;

	private:
		std::ifstream input_file;

};

#endif /* SRC_GRAPHPARSER_H_ */
