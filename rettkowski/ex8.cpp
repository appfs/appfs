/*
*  @file 		ex8.cpp
*  @details  	This file is the solution to exercise 8.
*  @author    	Alexander Rettkowski
*  @date      	12.07.2017
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

namespace exercise8
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
	exercise8::edge,
	(int, startNode)
	(int, endNode)
	(int, length)
)

namespace exercise8
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
* The function that calculates the shortest paths using Dijkstra's method.
* @param numberOfNodes Number of nodes in the graph.
* @param graph The graph represented as a vector auf edge-vectors.
* @param startNode The id of the node where the search starts.
*/
std::vector<int> dijkstra(int numberOfNodes, std::vector< std::vector< std::pair<int, int> > > graph, int startNode, std::vector<int>& predecessors)
{
	predecessors.resize(numberOfNodes, -1);
	std::vector<int> distanceTo(numberOfNodes, INT_MAX);
	std::priority_queue< std::pair<int, int>, std::vector< std::pair<int, int> >, std::greater< std::pair<int, int> > > queue;
	queue.push(std::pair<int, int>(startNode, 0));
	distanceTo[startNode] = 0;
	//predecessors[startNode] = -1;

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
				predecessors[compareNode] = currentNode;
			}
		}
	}
	return distanceTo;
}


/**
* This method checks (in a naive way), if a given number is prime or not.
* @param number The number to check.
* @returns True, if number is prime; false otherwise.
*/
bool isPrime(int number) {
	if (number == 2)
		return true;
	if (number % 2 == 0)
		return false;
	for (int i = 3; (i*i) <= number; i += 2) {
		if (number % i == 0) return false;
	}
	return true;
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
	typedef exercise8::line_parser<iterator_type> line_parser;
	line_parser parser;
	std::string currentLine;
	std::ifstream file(argv[1]);

	// get number of nodes
	char delimiter = ' ';
	getline(file, currentLine, delimiter);
	const int numberOfNodes = stoi(currentLine);
	getline(file, currentLine);
	int constSub = 1;

	std::vector<std::vector<std::pair<int, int>>> edges(numberOfNodes);
	while (getline(file, currentLine))
	{
		exercise8::edge parsedLine;
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
	






	std::vector<int> connectedVertices, unconnectedTerminals;
	for (int i = 0; i < numberOfNodes; i++)
	{
		if (isPrime(i))
			unconnectedTerminals.push_back(i);
	}

	connectedVertices.push_back(unconnectedTerminals.back());
	unconnectedTerminals.pop_back();

	std::map<int, std::vector<int>> distancesFromTerminals;
	std::map<int, std::vector<int>> predecessorsFromTerminals;
	for (int i : unconnectedTerminals)
	{
		std::vector<int> predecessors;
		std::vector<int> distancesFromI = dijkstra(numberOfNodes, edges, i, predecessors);
		distancesFromTerminals.insert(std::pair<int, std::vector<int>>(i, distancesFromI));
		predecessorsFromTerminals.insert(std::pair<int, std::vector<int>>(i, predecessors));
	}

	int steinerWeight = 0;

	while (unconnectedTerminals.size() > 0)
	{
		int connectingPointId = -1, connectingPointDistance = INT32_MAX;
		std::vector<int> path;

		int pathFrom = -1, pathTo = -1, mindist = INT32_MAX;
		for (int i = 0; i < unconnectedTerminals.size(); i++)
		{
			int vertexId = unconnectedTerminals[i];
			for (int j : connectedVertices)
			{
				if (distancesFromTerminals[vertexId][j] < mindist)
				{
					mindist = distancesFromTerminals[vertexId][j];
					pathTo = j;
					pathFrom = vertexId;
				}
			}
		}

		path.push_back(pathFrom);
		do
		{
			path.push_back(pathTo);
			pathTo = predecessorsFromTerminals[pathFrom][pathTo];
		} while (pathTo != pathFrom);

		// get path from 
		for (int i : path)
		{
			// if the vertex wasn't in the tree before, add
			if (std::find(connectedVertices.begin(), connectedVertices.end(), i) == connectedVertices.end())
			{
				connectedVertices.push_back(i);
			}
			// if it was an unconnected terminal, delete from the list
			auto pos = std::find(unconnectedTerminals.begin(), unconnectedTerminals.end(), i);
			if (pos != unconnectedTerminals.end())
			{
				unconnectedTerminals.erase(pos);
			}
		}

		steinerWeight += mindist;

	}

	std::cout << steinerWeight;

	return 0;
}

