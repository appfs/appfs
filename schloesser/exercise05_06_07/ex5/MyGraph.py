#!/usr/bin/env python3

'''
Standalone simple graph structure with the means to compute a longest shortest path.
'''
from misc import pop_next_destination

class Vertex:
    '''
    Vertex

    This class holds all relevant information about a vertex.
    '''

    def __init__(self, name):
        self.name = name
        self.edges_in = []
        self.edges_out = []

    def __str__(self):
        return "Vert({})".format(self.name)

    def __repr__(self):
        return self.__str__()

    def __lt__(self, other):
        return self.name < other.name

    def __gt__(self, other):
        return self.name > other.name

    def __ge__(self, other):
        return self.name >= other.name

    def __le__(self, other):
        return self.name <= other.name

    def add_incoming_edge(self, edge):
        '''
        Add an incoming edge.
        '''
        self.edges_in.append(edge)

    def add_outgoing_edge(self, edge):
        '''
        Add an outgoing adge.
        '''
        self.edges_out.append(edge)

    def get_incoming_edges(self):
        '''
        Return list of incoming edges.
        '''
        return self.edges_in

    def get_outgoing_edges(self):
        '''
        Return list of outgoing edges.
        '''
        return self.edges_out

    def get_name(self):
        '''
        Return name of Vertex
        '''
        return self.name

class Edge:
    '''
    Edge

    This class holds all relevant information about an edge.
    '''

    # source and target are Vertices, w is int
    def __init__(self, source, target, w):
        self.weight = w
        self.source = source
        self.target = target

    def register(self):
        '''
        Register edge to source and target vertices.
        '''
        self.source.add_outgoing_edge(self)
        self.target.add_incoming_edge(self)

    def __str__(self):
        return "Edge[{} => {}, {}]".format(self.source, self.target, self.weight)

    def __repr__(self):
        return self.__str__()

    def get_source(self):
        '''
        Return source of edge.
        '''
        return self.source

    def get_target(self):
        '''
        Return target of edge.
        '''
        return self.target

    def get_weight(self):
        '''
        Return weight of edge.
        '''
        return self.weight

class Graph:
    '''
    Graph

    This class holds all relevant information about a graph.
    '''

    def __init__(self, filename, directed=False):
        self.vertices = []
        self.edges = []
        self.n_vertices = 0
        self.n_edges = 0
        self.init_from_file(filename, directed)

    def __str__(self):
        return "Graph\nVertices: {}\nEdges: {}".format(
            self.vertices, "\n".join(map(lambda e: "\t"+e.__str__(), self.edges)))

    def __repr__(self):
        return self.__str__()

    def reset(self):
        '''
        Reset all information: vertices, edges, n_vertices, n_edges.
        '''
        self.vertices = []
        self.edges = []
        self.n_vertices = 0
        self.n_edges = 0

    def add_edge(self, vert1, vert2, weight):
        '''
        Add an edge to graph.

        Args:
            vert1 (int): source vertex
            vert2 (int): destination vertex
            weight (int): weight of edge.
        '''
        edge = Edge(vert1, vert2, weight)
        edge.register()
        self.edges.append(edge)
        self.n_edges = self.n_edges + 1

    def add_vertex(self, vert):
        '''
        Add a vertex to graph.

        Args:
            vert (int): Vertex name.
        '''
        self.vertices.append(Vertex(vert))
        self.n_vertices = self.n_vertices + 1

    def init_from_file(self, filename, directed=False):
        '''
        Read in a file containing a graph.
        First line contains two numbers: number of vertices, number of edges.
        Then the (weighted) edges are specified linewise: source target weight.

        Args:
            filename (string): name of file
            directed (boolean): default False for undirected graph, True for directed graph.
        '''
        self.reset()

        with open(filename, "r") as infile:
            # assume first line has number of vertices and number of edges
            line = infile.readline()
            info = line.split(" ")

            n_vert = int(info[0])
            #n_edges = int(info[1])

            # assume file contains vertices in range 1 .. n_vertices
            for i in range(n_vert):
                self.add_vertex(i+1)

            # assume file contains lines describing edges in form "vertex1 vertex2 weight"
            for i, line in enumerate(infile):
                info = line.split(" ")

                vert1 = self.vertices[int(info[0])-1]
                vert2 = self.vertices[int(info[1])-1]
                # assume w positive
                weight = int(info[2])

                self.add_edge(vert1, vert2, weight)
                if not directed:
                    self.add_edge(vert2, vert1, weight)

    def get_vertex_by_name(self, vertex):
        '''
        Return the name associated to the vertex.

        Args:
            vertex (string): the name of the vertex.

        Return:
            int: index of vertex with name vertex
        '''
        for vert in self.vertices:
            if vert.get_name() == vertex:
                return vert

    def get_min_unvisited(self, visited, distance):
        '''
        Return an unvisited vertex with current shortest distance amongst all unvisited vertices.

        Args:
            visited (array): an index-based array containing whether or not a vertex has been visited.
            distance (array): an index-based array containing the current shortest distances to a vertex.

        Return:
            int: index of nearest unvisited vertex.
        '''
        mind = -1
        minv = None
        for vert in self.vertices:
            if distance[vert] != -1 and not visited[vert] and (distance[vert] < mind or mind == -1):
                mind = distance[vert]
                minv = vert
        return minv

    def calculate_distances_to(self, vert):
        '''
        Calculate shortest distances to a vertex.

        Args:
            vert (int): Vertex to which the lengths of the shortest paths should be calculated.

        Return:
            array: containing shortest distance to each vertex.
        '''
        distance = {}
        visited = {}
        for vertex in self.vertices:
            distance[vertex] = -1
            visited[vertex] = False
        distance[vert] = 0
        to_visit = set()

        to_visit.add(vert)
        n_next = len(to_visit)
        while n_next != 0:
            curr = pop_next_destination(to_visit, distance)
            visited[curr] = True
            edges = curr.get_incoming_edges()
            for edge in edges:
                neighbor = edge.get_source()
                if not visited[neighbor]:
                    # update distances
                    newdist = distance[curr] + edge.get_weight()
                    if distance[neighbor] == -1 or newdist < distance[neighbor]:
                        distance[neighbor] = newdist
                    to_visit.add(neighbor)
            n_next = len(to_visit)
        return distance

    def get_longest_shortest_to(self, vert):
        '''
        Return the length and start vertex of a shortest path to vert.
        If more than one path hast shortest length, the one with the smallest index is returned.

        Args:
            vert (int): vertex to which a shortest path is being searched.

        Return:
            (int, int): pair of vertex and distance.
        '''
        vert = self.get_vertex_by_name(vert)
        distances = self.calculate_distances_to(vert)
        max_dist = 0
        max_vert = vert
        for vertex in sorted(self.vertices):
            dist = distances[vertex]
            if dist > max_dist:
                max_dist = dist
                max_vert = vertex
        return (max_vert.get_name(), max_dist)
