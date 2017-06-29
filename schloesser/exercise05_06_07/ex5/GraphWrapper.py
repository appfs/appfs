#!/usr/bin/env python

'''
Wrapper class for graph-tool to compute longest shortest path to a given node.
'''
from graph_tool.all import load_graph_from_csv
from misc import pop_next_destination

class GraphWrapper:
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
