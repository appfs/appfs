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
	inputFile.open(file);
	openedSuccessfully = !inputFile.fail();
}

bool GraphParser::readFirstLine(int& numVertices, int& numEdges) {

	string line;
	getline(inputFile, line);	//first line is read

	auto it = line.begin();
	int n, m;
	bool success = phrase_parse(it, line.end(),
			int_[([&n](int j){ n = j; })]
				 >> int_[([&m](int j){ m = j; })], space);

	if (!success || it != line.end()) return false;
	numVertices = n;
	numEdges = m;

	return true;
}


/*
 * Implementation of the graph parser read method by means of boost::spirit
 */
bool GraphParser::readEdgeData(Edge edges[], double weights[]) {


	assert(NULL != edges);
	assert(NULL != weights);


	// we use boost::spirit to parse each line //
	string line;

	int i = 0;

	//read line by line
	while (getline(inputFile, line)) {

		auto it = line.begin();
		int start;
		int end;
		double weight;
		bool success = phrase_parse(it, line.end(),
				int_[([&start](int j){ start = j; })]
					 >> int_[([&end](int j){ end = j; })]
							 >> double_[([&weight](double j){ weight = j; })], space);
		if (success && it == line.end()) {
			edges[i] = Edge(start-1, end-1);
			weights[i] = weight;
		} else {
			return false;
		}
		++i;
	}


	return true;
}



