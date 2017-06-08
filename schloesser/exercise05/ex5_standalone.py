#!/usr/bin/env python 
            irint(to_visit, curr)

import sys

class Vertex:

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
        self.edges_in.append(edge)

    def add_outgoing_edge(self, edge):
        self.edges_out.append(edge)
    
    def get_incoming_edges(self):
        return self.edges_in

    def get_outgoing_edges(self):
        return self.edges_out

    def get_name(self):
        return self.name
    
class Edge:

    # source and target are Vertices, w is int
    def __init__(self, source, target, w):
        self.weight = w
        self.source = source
        self.target = target

    def register(self):
        self.source.add_outgoing_edge(self)
        self.target.add_incoming_edge(self)

    def __str__(self):
        return "Edge[{} => {}, {}]".format(self.source, self.target, self.weight)

    def __repr__(self):
        return self.__str__()
    
    def get_source(self):
        return self.source

    def get_target(self):
        return self.target

    def get_weight(self):
        return self.weight

class Graph:

    def __init__(self, filename, directed=False):
        self.init_from_file(filename, directed)

    def __str__(self):
        return "Graph\nVertices: {}\nEdges: {}".format(self.vertices, "\n".join(map(lambda e: "\t"+e.__str__(), self.edges)))

    def __repr__(self):
        return self.__str__()
    
    def reset(self):
        self.vertices = []
        self.edges = []
        self.n_vertices = 0
        self.n_edges = 0

    def add_edge(self, v1, v2, w):
        e = Edge(v1, v2, w)
        e.register()
        self.edges.append(e)
        self.n_edges = self.n_edges + 1

    def add_vertex(self, v): 
        self.vertices.append(Vertex(v))
        self.n_vertices = self.n_vertices + 1

    def init_from_file(self, filename, directed=False):
        self.reset()

        with open(filename, "r") as f:\
            # assume first line has number of vertices and number of edges
            line = f.readline()
            info = line.split(" ")

            n_vert = int(info[0]) 
            n_edges = int(info[1])

            # assume file contains vertices in range 1 .. n_vertices
            for i in range(n_vert):
                self.add_vertex(i+1)
            
            # assume file contains lines describing edges in form "vertex1 vertex2 weight"
            for i, line in enumerate(f):
                info = line.split(" ")
                
                v1 = self.vertices[int(info[0])-1]
                v2 = self.vertices[int(info[1])-1]
                # assume w positive
                w = int(info[2])
                
                self.add_edge(v1, v2, w)
                if not directed:
                    self.add_edge(v2, v1, w)

    def get_vertex_by_name(self, vertex):
        # vertex is a string
        for v in self.vertices:
            if v.get_name() == vertex:
                return v

    def get_min_unvisited(self, visited, distance):
        mind = -1
        minv = None
        for v in self.vertices:
            if distance[v] != -1 and not visited[v] and (distance[v] < mind or mind == -1):
                mind = distance[v]
                minv = v
        return minv

    def pop_next_destination(self, to_visit, distance):
        mind = -1
        minv = None
        for v in to_visit:
            # assert distance[v] >= 0
            if distance[v] < mind or mind == -1:
                mind = distance[v]
                minv = v
        to_visit.remove(minv)
        return minv

    def calculate_distances_to(self, vert):
        distance = {}
        visited = {}
        for vertex in self.vertices:
            distance[vertex] = -1
            visited[vertex] = False
        distance[vert] = 0
        to_visit = set()
        
        to_visit.add(vert)
        while len(to_visit) != 0:
            curr = self.pop_next_destination(to_visit, distance)
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
        return distance
    
    def get_longest_shortest_to(self, vert):
        distances = self.calculate_distances_to(vert)
        max_dist = 0
        max_vert = vert
        for v in sorted(self.vertices):
            dist = distances[v]
            if dist > max_dist:
                max_dist = dist
                max_vert = v
        return (max_vert, max_dist)

if __name__ == "__main__":
    # read filename from commandlineargs
    filename = sys.argv[1]
    #vert = int(sys.argv[2])
    vert = int(1)
    g = Graph(filename, False)

    vert = g.get_vertex_by_name(vert)
    res = g.get_longest_shortest_to(vert)
    
    print("RESULT VERTEX {}\n\nRESULT DIST {}".format(res[0].get_name(), res[1]))
