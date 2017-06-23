#!/usr/bin/env python

'''
Exercises 5, 6 and 7

This package provides means to solving exercises 5 to 7 from the lecture appfs.
'''
import sys
import time
from graph_tool.all import load_graph_from_csv

def pop_next_destination(to_visit, distance):
    '''
    Return a vertex that still has to be visited and has currently smallest distance to startvertex.

    Args:
        to_visit (array): an array of bools indicating whether or not a vertex has to be visited.
        distance (array): an array containing the current shortest distances to the corresponding vertices.
    '''
    mind = -1
    minv = None
    for vert in to_visit:
        if distance[vert] < mind or mind == -1:
            mind = distance[vert]
            minv = vert
    to_visit.remove(minv)
    return minv

class Ex5:
    '''
    A class containing methods to read in a graph and calculate the longest shortest path to a particular vertex.
    '''

    def __init__(self, filename, directed=False):
        self.graph = load_graph_from_csv(filename, directed=directed, csv_options={"delimiter": " ", "quotechar": '"'},
                                         skip_first=True, string_vals=True)
        self.vertex_names = self.graph.vertex_properties["name"]

    def calculate_distances_to(self, vert):
        '''
        Calculate shortest distances to a vertex.

        Args:
            vert (int): Vertex to which the lengths of the shortest paths should be calculated.

        Return:
            array: containing shortest distance to each vertex.
        '''
        directed = self.graph.is_directed()
        distance = {}
        visited = {}
        for vertex in self.graph.get_vertices():
            distance[vertex] = -1
            visited[vertex] = False
        distance[vert] = 0
        to_visit = set()

        weights = self.graph.edge_properties["c0"]
        to_visit.add(vert)

        n_to_visit = len(to_visit)
        while n_to_visit != 0:
            curr = pop_next_destination(to_visit, distance)
            visited[curr] = True
            if directed:
                edges = self.graph.get_in_edges(curr)
            else:
                edges = self.graph.get_out_edges(curr)
            for edge in edges:
                if directed:
                    neighbor = edge[0]
                else:
                    neighbor = edge[1]
                if not visited[neighbor]:
                    # update distances
                    newdist = distance[curr] + int(weights[self.graph.edge(edge[0], edge[1])])
                    if distance[neighbor] == -1 or newdist < distance[neighbor]:
                        distance[neighbor] = newdist
                    to_visit.add(neighbor)
            n_to_visit = len(to_visit)
        return distance

    def vert_index(self, vert):
        '''
        Return index of a particular vertex.

        Args:
            vert (name): vertex name

        Return:
            int: index of vertex
        '''
        for vertex in range(self.graph.num_vertices()):
            if int(self.vertex_names[vertex]) == vert:
                return vertex

    def vert_name(self, vert):
        '''
        Return the name of a particular vertex.

        Args:
            vert (int): index of vertex.

        Return:
            int: name of vertex.
        '''
        return int(self.vertex_names[vert])

    def get_longest_shortest_to(self, vert):
        '''
        Return the length and start vertex of a shortest path to vert.
        If more than one path hast shortest length, the one with the smallest index is returned.

        Args:
            vert (int): vertex to which a shortest path is being searched.

        Return:
            (int, int): pair of vertex and distance.
        '''
        # name to index
        vert = self.vert_index(vert)
        distances = self.calculate_distances_to(vert)
        max_dist = 0
        max_vert = vert
        for vertex in self.graph.get_vertices():
            dist = distances[vertex]
            if dist > max_dist or (dist == max_dist and self.vert_name(vertex) < self.vert_name(max_vert)):
                max_dist = dist
                max_vert = vertex
        return (self.vert_name(max_vert), max_dist)

if __name__ == "__main__":
    T0CLOCK = time.perf_counter()
    T0TIME = time.process_time()
    # read filename from commandlineargs
    FILE = sys.argv[1]
    VERT = 1

    EX = Ex5(filename=FILE, directed=False)
    RES = EX.get_longest_shortest_to(VERT)

    print("RESULT VERTEX {}\n\nRESULT DIST {}".format(RES[0], RES[1]))
    T1CLOCK = time.perf_counter()
    T1TIME = time.process_time()
    print("Program took {} s (wallclock time)".format(T1CLOCK - T0CLOCK))
    print("Program took {} s (process time)".format(T1TIME - T0TIME))
