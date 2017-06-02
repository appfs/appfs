#!/usr/bin/env python 

import sys

class Vertex:

    def __init__(self, name):
        self.name = name
        self.edges_in = []
        self.edges_out = []

    def __str__(self):
        return "V({})".format(self.name)
    
    def __repr__(self):
        return self.__str__()

    def add_incoming_edge(self, edge):
        self.edges_in.append(edge)

    def add_outgoing_edge(self, edge):
        self.edges_out.append(edge)

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
        return "E[{} => {}, {}]".format(self.source, self.target, self.weight)

    def __repr__(self):
        return self.__str__()
    
class Graph:

    def __init__(self, filename):
        self.init_from_file(filename)

    def __str__(self):
        return "Vertices: {}\nEdges: {}".format(self.vertices, "\n".join(map(lambda e: "\t"+e.__str__(), self.edges)))

    def __repr__(self):
        return self.__str__()
    
    def reset(self):
        self.vertices = []
        self.edges = []

    def add_edge(self, v1, v2, w):
        e = Edge(v1, v2, w)
        e.register()
        self.edges.append(e)

    def add_vertex(self, v): 
        self.vertices.append(Vertex(v))

    def init_from_file(self, filename):
        self.reset()

        with open(filename, "r") as f:
            line = f.readline()
            info = line.split(" ")

            n_vert = int(info[0]) 
            n_edges = int(info[1])

            for i in range(n_vert):
                self.add_vertex(i+1)
            
            for i, line in enumerate(f):
                info = line.split(" ")
                
                v1 = self.vertices[int(info[0])-1]
                v2 = self.vertices[int(info[1])-1]
                w = int(info[2]) 
                
                self.add_edge(v1, v2, w)

if __name__ == "__main__":
    # read filename from commandlineargs
    filename = sys.argv[1]
    g = Graph(filename)

    print(g)
