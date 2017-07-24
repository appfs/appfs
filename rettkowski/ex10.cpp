/*
*  @file 		ex10.cpp
*  @details  	This file is the solution to exercise 9.
*  @author    	Alexander Rettkowski
*  @date      	20.07.2017
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

#include <omp.h>

using namespace boost;

namespace exercise9
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
	exercise9::edge,
	(int, startNode)
	(int, endNode)
	(int, length)
)

namespace exercise9
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
* The function that calculates the shortest paths using Dijkstra's method.
* @param numberOfNodes Number of nodes in the graph.
* @param graph The graph represented as a vector auf edge-vectors.
* @param startNode The id of the node where the search starts.
* @returns The weight of the Steiner Tree and the edge list of the tree coupled as a std::pair
*/
std::pair<int, std::list<std::pair<int, int>>>  steinerTree(int numberOfNodes, std::vector< std::vector< std::pair<int, int> > > graph, int startNode, std::vector<bool> primes)
{
	std::vector<int> connectedVertices;
	connectedVertices.push_back(startNode);
	std::list<std::pair<int, int>> edgeList;

	std::vector<int> predecessors(numberOfNodes, -1);
	std::vector<int> distanceTo(numberOfNodes, INT_MAX);
	std::priority_queue< std::pair<int, int>, std::vector< std::pair<int, int> >, std::greater< std::pair<int, int> > > queue;
	queue.push(std::pair<int, int>(startNode, 0));
	distanceTo[startNode] = 0;
	//predecessors[startNode] = -1;
	int steinerWeight = 0;

	int currentNode, compareNode, compareNodeDistance, currentNodeDistance;

	while (!queue.empty()) {
		currentNode = queue.top().first;
		currentNodeDistance = queue.top().second;
		queue.pop();

		if (distanceTo[currentNode] < currentNodeDistance) continue;

		// TODO: Kann hier parallelisiert werden?
		for (int i = 0; i < graph[currentNode].size(); i++) {
			compareNode = graph[currentNode][i].first;
			compareNodeDistance = graph[currentNode][i].second;
			if (distanceTo[compareNode] > distanceTo[currentNode] + compareNodeDistance) {
				if (primes[compareNode])
				{
					predecessors[compareNode] = currentNode;

					int backtracknode = compareNode;
					while (std::find(connectedVertices.begin(), connectedVertices.end(), backtracknode) == connectedVertices.end())
					{
						for (std::pair<int, int> edge : graph[predecessors[backtracknode]])
						{
							if (edge.first == backtracknode)
							{
								edgeList.push_back(std::pair<int, int>(predecessors[backtracknode], backtracknode));
								steinerWeight += edge.second;
							}
						}

						distanceTo[backtracknode] = 0;
						connectedVertices.push_back(backtracknode);
						backtracknode = predecessors[backtracknode];
						queue.push(std::pair<int, int>(backtracknode, 0));
					}
				}
				else
				{
					distanceTo[compareNode] = distanceTo[currentNode] + compareNodeDistance;
					queue.push(std::pair<int, int>(compareNode, distanceTo[compareNode]));
					predecessors[compareNode] = currentNode;
				}
			}
		}
	}


	return std::pair<int, std::list<std::pair<int, int>>>(steinerWeight, edgeList);
}

/**
* A recursively implemented check if a graph (given as an edge list) contains a cycle
* @param edgeList The edge list representing the graph
* @param startPoint ID of the node where the traversal starts
* @param visisted a vector of already visited edge ID's
* @returns True, if the graph contains a cycle; false otherwise.
*/
bool hasCycle(std::list<std::pair<int,int> > edgeList, int startPoint, std::vector<int> visited)
{
	std::vector<int> soFar = visited;

	if (std::find(visited.begin(), visited.end(), startPoint) != visited.end())
		return true;

	soFar.push_back(startPoint);

	for (std::pair<int, int> edge : edgeList)
	{
		if (edge.first == startPoint)
			if (hasCycle(edgeList, edge.second, soFar))
				return true;
	}

	return false;
}

/**
* A simple method that checks if a graph contains all nodes with prime ID's.
* @param edgeList The edge list representing the graph
* @param primes The list of primes the graph should contain
* @returns True, if the graph contains all primes; false otherwise.
*/
bool containsAllPrimes(std::list<std::pair<int, int> > edgeList, std::vector<bool> primes)
{
	std::vector<bool> isIncluded(primes.size(), false);
	
	for (std::pair<int, int> edge : edgeList)
	{
		isIncluded[edge.first] = true;
		isIncluded[edge.second] = true;
	}

	for (int i = 0; i < isIncluded.size(); i++)
	{
		if (primes[i] && !isIncluded[i])
			return false;
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
	typedef exercise9::line_parser<iterator_type> line_parser;
	line_parser parser;
	std::string currentLine;
	std::ifstream file(argv[1]);

	// get number of nodes
	char delimiter = ' ';
	getline(file, currentLine, delimiter);
	const int numberOfNodes = stoi(currentLine);
	getline(file, currentLine);
	int constSub = 1;

	std::vector<bool> primes(numberOfNodes, false);
	for (int i = 0; i < numberOfNodes; i++)
	{
		if (isPrime(i))
		{
			primes[i] = true;
		}
	}

	std::vector<std::vector<std::pair<int, int>>> edges(numberOfNodes);
	while (getline(file, currentLine))
	{
		exercise9::edge parsedLine;
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

	timer::cpu_timer runTimer;
	omp_set_dynamic(0);
	if (argc >= 5)
	{
		omp_set_num_threads(std::stoi(argv[4]));
	}
	else
	{
		omp_set_num_threads(4);
	}

	bool printTree = false;
	if (argc >= 4)
	{
		std::string test = argv[3];
		if (0 == test.compare("-s"))
			printTree = true;
	}

	std::vector<int> terminalIds;
	for (int i = 0; i < numberOfNodes; i++)
	{
		if (primes[i])
			terminalIds.push_back(i);
	}

	int numberOfStartingPoints = std::stoi(argv[2]);
	std::vector<int> lengths(numberOfStartingPoints);
	std::vector<std::list<std::pair<int, int>>> edgelists(numberOfStartingPoints);

#pragma omp parallel for
	for (int i = 0; i < numberOfStartingPoints; i++)
		{
		    std::pair<int, std::list<std::pair<int, int>>> currentRun = steinerTree(numberOfNodes, edges, terminalIds[i], primes);
			lengths[i] = currentRun.first;
			edgelists[i] = currentRun.second;
			//std::cout << "Length starting from " << i << ": " << lengths[i] << std::endl;
		}

	int minlength = INT32_MAX;
	int minId = -1;
	std::list<std::pair<int, int>> minList;
	for (int i = 0; i < numberOfStartingPoints; i++)
	{
		if (lengths[i] < minlength)
		{
			minlength = lengths[i];
			minId = i;
			minList = edgelists[i];
		}
	}

	timer::cpu_times runtime = boostTimer.elapsed();
	//std::list<std::pair<int, int>> edgelist = steinerTree(numberOfNodes, edges, minId, primes).second;


	std::cout << "TLEN: " << minlength << std::endl;
	if (printTree)
	{
		std::cout << "TREE: ";

		for (std::pair<int, int> edge : minList)
		{
			std::cout << "(" << edge.first << ", " << edge.second << ") ";
		}
		std::cout << std::endl;
	}

	std::vector<int> visited(numberOfNodes);
	//assert(!hasCycle(edgelist, 1, visited) && containsAllPrimes(edgelist, primes));

	std::list<std::pair<int, int>> invalidList(3);
	invalidList.push_back(std::pair<int, int>(1, 2));
	invalidList.push_back(std::pair<int, int>(2, 3));
	invalidList.push_back(std::pair<int, int>(3, 1));
	bool noChance1 = hasCycle(invalidList, 1, visited);
	bool noChance2 = containsAllPrimes(invalidList, primes);


	timer::cpu_times time = boostTimer.elapsed();
	std::cout << "TIME: " << (time.user + time.system) / 1e9 << "s\n";
	std::cout << "WALL: " << runtime.wall / 1e9 << "s\n";


	return 0;
}

