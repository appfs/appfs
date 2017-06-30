/*
*  @file 		ex5.cpp
*  @details  	This file is the solution to exercise 5.
*  @author    	Alexander Rettkowski
*  @date      	08.06.2017
*/
#include <boost/config/warning_disable.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/fusion/include/io.hpp>

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths_no_color_map.hpp>
#include <boost/property_map/property_map.hpp>

#include <iostream>
#include <string>
#include <complex>
#include <fstream>


using namespace boost;

namespace exercise5
{
	namespace qi = boost::spirit::qi;
	namespace ascii = boost::spirit::ascii;
	struct edge
	{
		int startNode;
		int endNode;
		int length;
	};
}


BOOST_FUSION_ADAPT_STRUCT(
	exercise5::edge,
	(int, startNode)
	(int, endNode)
	(int, length)
)

namespace exercise5
{
	template <typename Iterator>
	struct line_parser : qi::grammar<Iterator, edge()>
	{
		line_parser() : line_parser::base_type(start)
		{
			using qi::int_;
			start %= int_ >> ' ' >> int_ >> ' ' >> int_;
		}

		qi::rule<Iterator, edge()> start;
	};
}

/**
* The main function that reads in a file and processes it.
* @param argc Number of command line arguments.
* @param *argv a pointer to the array of command line arguments.
*/
int main(int argc, char *argv[])
{
	typedef adjacency_list < listS, vecS, undirectedS, no_property, property < edge_weight_t, int > > graph_t;
	typedef graph_traits < graph_t >::vertex_descriptor vertex_descriptor;
	typedef std::pair<int, int> Edge;

	std::vector<Edge> edges;
	std::vector<int> weights;
	
	using boost::spirit::ascii::space;
	typedef std::string::const_iterator iterator_type;
	typedef exercise5::line_parser<iterator_type> line_parser;
	line_parser parser;
	std::string currentLine;
	std::ifstream file(argv[1]);

	// get number of nodes
	char delimiter = ' ';
	getline(file, currentLine, delimiter);
	int numberOfNodes = std::stoi(currentLine);
	getline(file, currentLine);
	int constSub = 1;
	while (getline(file, currentLine))
	{
		exercise5::edge parsedLine;
		std::string::const_iterator currentPosition = currentLine.begin();
		std::string::const_iterator lineEnd = currentLine.end();
		bool parsingSucceeded = phrase_parse(currentPosition, lineEnd, parser, space, parsedLine);
		
		if (parsingSucceeded && currentPosition == lineEnd)
		{
			edges.push_back(Edge(parsedLine.startNode-constSub, parsedLine.endNode-constSub));
			weights.push_back(parsedLine.length);
		}
	}

	file.close();

	graph_t g(edges.data(), edges.data() + edges.size(), weights.data(), numberOfNodes);

	property_map<graph_t, edge_weight_t>::type weightmap = get(edge_weight, g);
	std::vector<vertex_descriptor> p(num_vertices(g));
	std::vector<int> d(num_vertices(g));
	vertex_descriptor s = vertex(0, g);

	dijkstra_shortest_paths_no_color_map(g, s, predecessor_map(make_iterator_property_map(p.begin(), get(vertex_index, g))).
			distance_map(make_iterator_property_map(d.begin(), get(vertex_index, g))));

	graph_traits < graph_t >::vertex_iterator vi, vend;
	size_t vertex = -1;
	double distance = -1;
	for (tie(vi, vend) = vertices(g); vi != vend; ++vi) {
		if (d[*vi] > distance)
		{
			distance = d[*vi];
			vertex = *vi;
		}
		if ((d[*vi] == distance) && (*vi > vertex))
		{
			vertex = *vi;
		}
	}

	std::cout << "RESULT VERTEX " << vertex << std::endl;
	std::cout << "RESULT DIST " << distance << std::endl;

	return 0;
}
