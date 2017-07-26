/*
*  @file 		ex7.cpp
*  @details  	This file is the solution to exercise 7.
*  @author    	Alexander Rettkowski
*  @date      	28.06.2017
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

#include <boost/timer/timer.hpp>
#include <boost/chrono.hpp>

#include <iostream>
#include <string>
#include <complex>
#include <fstream>
#include <queue> 

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
* The function that calculates the longest shortest path from node 0.
* @param numberOfNodes Number of nodes in the graph.
* @param graph The graph represented as a vector auf edge-vectors.
*/
std::pair<int, int> dijkstra( int numberOfNodes, std::vector< std::vector< std::pair<int, int> > > graph)
{
	std::vector<int> distanceTo(numberOfNodes, INT_MAX);
	std::priority_queue< std::pair<int, int>, std::vector< std::pair<int, int> >, std::greater< std::pair<int, int> > > queue;
	queue.push(std::pair<int, int>(0, 0));
	distanceTo[0] = 0;

	int currentNode, compareNode, compareNodeDistance, currentNodeDistance;

	while (!queue.empty()) {
		currentNode = queue.top().first;
		currentNodeDistance = queue.top().second;
		queue.pop();

		if (distanceTo[currentNode] < currentNodeDistance) continue;

		for (int i = 0; i < graph[currentNode].size(); i++) {
			compareNode = graph[currentNode][i].first;
			compareNodeDistance = graph[currentNode][i].second;
			if (distanceTo[compareNode] > distanceTo[currentNode] + compareNodeDistance) {
				distanceTo[compareNode] = distanceTo[currentNode] + compareNodeDistance;
				queue.push(std::pair<int, int>(compareNode, distanceTo[compareNode]));
			}
		}
	}

	std::pair<int, int> solution(-1, -1);
	for (int i = 0; i < numberOfNodes; i++)
	{
		if (distanceTo[i] > solution.second)
		{
			solution.second = distanceTo[i];
			solution.first = i;
		}
		if ((distanceTo[i] == solution.second) && (i > solution.first))
		{
			solution.first = i;
		}
	}

	return solution;
}

/**
* The main function that reads in a file and processes it.
* @param argc Number of command line arguments.
* @param *argv a pointer to the array of command line arguments.
*/
int main(int argc, char *argv[])
{
	timer::cpu_timer boostTimer;

	using boost::spirit::ascii::space;
	typedef std::string::const_iterator iterator_type;
	typedef exercise5::line_parser<iterator_type> line_parser;
	line_parser parser;
	std::string currentLine;
	std::ifstream file(argv[1]);

	// get number of nodes
	char delimiter = ' ';
	getline(file, currentLine, delimiter);
	const int numberOfNodes = std::stoi(currentLine);
	getline(file, currentLine);
	int constSub = 1;

	std::vector<std::vector<std::pair<int, int>>> edges(numberOfNodes);
	while (getline(file, currentLine))
	{
		exercise5::edge parsedLine;
		std::string::const_iterator currentPosition = currentLine.begin();
		std::string::const_iterator lineEnd = currentLine.end();
		bool parsingSucceeded = phrase_parse(currentPosition, lineEnd, parser, space, parsedLine);

		if (parsingSucceeded && currentPosition == lineEnd)
		{
			std::pair<int, int> *tempEdge = new std::pair<int, int>();
			tempEdge->first = parsedLine.endNode - constSub;
			tempEdge->second = parsedLine.length;
			edges[parsedLine.startNode - constSub].push_back(*tempEdge);

			std::pair<int, int> *reverseEdge = new std::pair<int, int>();
			reverseEdge->first = parsedLine.startNode - constSub;
			reverseEdge->second = parsedLine.length;
			edges[parsedLine.endNode - constSub].push_back(*reverseEdge);
		}
	}

	file.close();
	
	std::pair<int, int> solution = dijkstra(numberOfNodes, edges);
	int vertex = solution.first, distance = solution.second;

	std::cout << "RESULT VERTEX " << vertex << std::endl;
	std::cout << "RESULT DIST " << distance << std::endl;

	timer::cpu_times time = boostTimer.elapsed();
	std::cout << "CPU TIME: " << (time.user + time.system) / 1e9 << "s\n";
	std::cout << "WALL CLOCK TIME: " << time.wall / 1e9 << "s\n";

	return 0;
}

