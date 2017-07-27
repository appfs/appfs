/**
 * @file 	GraphParser.cpp
 * @author 	Fabian Wegscheider
 * @date 	Jul 14, 2017
 */

#include <boost/spirit/include/qi.hpp>
#include "GraphParser.h"

using std::string;
using namespace boost::spirit;
using qi::int_;
using qi::double_;
using qi::phrase_parse;
using ascii::space;


GraphParser::GraphParser(string file) {
	input_file.open(file);
	opened_successfully = !input_file.fail();
}


bool GraphParser::read_first_line(int& num_vertices, int& num_edges) {

	string line;
	getline(input_file, line);	//first line is read

	auto it = line.begin();
	int n, m;
	bool success = phrase_parse(it, line.end(),
			int_[([&n](int j){ n = j; })]
				 >> int_[([&m](int j){ m = j; })], space);

	if (!success || it != line.end()) return false;
	num_vertices = n;
	num_edges = 2*m;

	return true;
}


/*
 * Implementation of the graph parser read method by means of boost::spirit
 */
bool GraphParser::read_edge_data(Edge edges[], weight_type weights[]) {

	assert(NULL != edges);
	assert(NULL != weights);

	//we use boost::spirit to parse each line
	string line;

	int i = 0;

	//read line by line
	while (getline(input_file, line)) {

		auto it = line.begin();
		int start;
		int end;
		double weight;
		bool success = phrase_parse(it, line.end(),
				int_[([&start](int j){ start = j; })]
					 >> int_[([&end](int j){ end = j; })]
							 >> double_[([&weight](weight_type j){ weight = j; })], space);
		if (success && it == line.end()) {
			//since graph is undirected we add both directions for each edge
			edges[2*i] = Edge(start-1, end-1);
			edges[2*i+1] = Edge(end-1, start-1);
			weights[2*i] = weight;
			weights[2*i+1] = weight;
		} else {
			return false;
		}
		++i;
	}

	return true;
}



