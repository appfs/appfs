#!/usr/bin/env python3

import sys
import numpy as np
import matplotlib.pyplot as plt
from collections import defaultdict, deque, OrderedDict
from random import shuffle, choice


class Graph():
    """This class provides the basic functionality for graph handling."""
    def __init__(self):
        """Initialize the different containers for nodes, edges and dists."""
        self.nodes = set()
        self.edges = defaultdict(list)
        self.distances = {}

    def addNode(self, value):
        """Add a node to the graph."""
        self.nodes.add(value)

    def addEdge(self, from_node, to_node, distance):
        """
        Store the connection between node A and node B with a certain distance.
        """
        self.edges[from_node].append(to_node)
        self.edges[to_node].append(from_node)
        self.distances[(from_node, to_node)] = distance
        # self.distances[(to_node, from_node)] = -distance

    def getNeighboursOf(self, node):
        """
        Find the direct neighbours of a given node. Might be usefull for larger
        sets of nodes with fewer interconnections.
        """
        neighbours = []
        for a, b in self.distances.keys():
            if a == node:
                neighbours.append(b)

        return neighbours


class Dijsktra():
    """
    Provides mainly two functions to receive the path from Dijkstras algortihm:
    dijkstra, getPath. The rest is performance.
    """

    def __init__(self, filename, start):
        """
        Initialise the Graph class and start running the script.

        Parameters
        ----------
        filename : graph file to extract the path from
        start : the node to begin with [default = 1]
        (graph : internally instanciated)

        Example
        -------
        d = Dijkstra('world666.gph', 1)
        d.run()
        """
        self.graph = Graph()
        self.filename = filename
        self.start = start

    def run(self):
        """
        Execute the task:
        - Read in graph file
        - search paths with dijkstra
        - search for the path
        """
        N, conns, nodesA, nodesB, weight = self.readGPH(self.filename)
        print("{} nodes with {} connections".format(N, conns))

        # prepare the graph with nodes and edges
        [self.graph.addNode(i+1) for i in range(N)]
        [self.graph.addEdge(nodesA[i], nodesB[i], weight[i]) for i in range(len(nodesA))]

        # nodes_taken = []
        path_taken = []
        dist_taken = []
        # search all paths
        self.visited, self.paths = self.dijkstra(self.start)
        # print("dijkstra finished")
        # print(nodesB, len(nodesB), len(set(nodesB)))
        # sys.exit()
        dist = list(self.visited.values())
        idx = dist.index(max(dist))
        max_dist = dist[idx]
        node = list(self.visited.keys())[idx]
        print(max_dist, node)
        # for i in self.visited.keys():
        #     if i == self.start:
        #         continue
        #     # now find for every node the longest way to the start node
        #     self.getPath()

        # track = self.getPath2(self.start, 23)
        # print(track)
        # print(self.visited.keys())

        # nodes_taken.append(len(path))
        # path_taken.append(track)
        # dist_taken.append(visited[i])

        # max_dist = max(dist_taken)
        # idx = dist_taken.index(max_dist)
        # # prompt output
        # print("RESULT VERTEX {}".format(path_taken[idx][-1]))
        # print("RESULT DIST {}".format(max_dist))

    def readGPH(self, filename):
        """
        Read and process a gph-file.

        Parameters
        ----------
        (filename : graph file to extract the path from)
        """
        # get the first line with n nodes and n connections
        with open(filename, mode='r') as gph:
            for line in gph:
                header = line.split(' ')
                if len(header) == 2:
                    nodes_ = int(header[0])
                    conns = int(header[1])
                    break

        # reopen and get all edges with respective weights
        data = np.genfromtxt(filename, dtype=int, comments='#', delimiter=' ', skip_header=1)
        return nodes_, conns, data[:, 0], data[:, 1], data[:, 2]

    def dijkstra(self, source):
        """
        Scan the given graph from source.

        Parameters
        ----------
        (source : the source node where the search starts)
        """
        unvisited = set(self.graph.nodes)
        visited = {source: 0}
        paths = {}

        while unvisited:
            min_node = None
            for node in unvisited:
                if node in visited:
                    if min_node is None:
                        min_node = node
                    elif visited[node] < visited[min_node]:
                        min_node = node

            if min_node is None:
                break

            unvisited.remove(min_node)
            # for edge in graph.getNeighboursOf(min_node):
            for edge in self.graph.edges[min_node]:  # less elegant but faster
                try:
                    weight = visited[min_node] + self.graph.distances[(min_node, edge)]
                    # edge_is_found = True
                    # print("{}-{} found".format(edge, min_node))

                except KeyError:
                    # landing here if desired edge doesn't exist in distances
                    # print("{}-{} not found".format(min_node, edge))
                    continue

                # if edge_is_found:
                else:
                    if edge not in visited or weight < visited[edge]:
                        visited[edge] = weight
                        paths[edge] = min_node

        # print(visited, paths)
        # sys.exit()
        return visited, paths

    def getPath(self):  # , source, dest):
        """Search for the path as given from dijkstra algorithm."""
        # print(self.visited)
        # print(self.paths)



        # track = deque()
        # # print(source, dest)
        # destination = self.paths[dest]
        #
        # while destination != source:
        #     track.appendleft(destination)
        #     destination = self.paths[destination]
        #
        # track.appendleft(source)
        # track.append(dest)
        #
        # return list(track)

    def getPath2(self, source, dest):
        """
        Search for the path as given from dijkstra algorithm.

        Parameters
        ----------
        source : start node
        dest : end node
        """
        track = deque()
        # print(source, dest)
        destination = self.paths[dest]

        while destination != source:
            track.appendleft(destination)
            destination = self.paths[destination]

        track.appendleft(source)
        track.append(dest)

        return list(track)


def main(argv):
    """Argument handler."""
    import argparse
    parser = argparse.ArgumentParser(
        formatter_class=argparse.RawDescriptionHelpFormatter,
        description=
            '''\
                     EXERCISE 5
            ----------------------------
            find the longest shortest
            path with the dijkstra
            algorithm in a set of nodes.
            '''
        )

    parser.add_argument('file', help='graph file to search through', action='store')
    parser.add_argument('-s', '--start', help='node name to start with', action='store', type=int, default=1, required=False)

    args = parser.parse_args()
    d = Dijsktra(args.file, args.start)
    d.run()


if __name__ == '__main__':
    main(sys.argv[1:])
