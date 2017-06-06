#!/usr/bin/env python 

import sys

class Element:
    
    # obj can be anything, value can be used as a priorty
    def __init__(self, obj, value):
        self.obj = obj
        self.value = value
        self.successor = None
        self.predecessor = None

    def __str__(self):
        return "Elem({}, {})".format(self.value, self.obj)

    def __repr__(self):
        return self.__str__()
    
    def set_successor(self, successor):
        self.successor = successor
    
    def set_predecessor(self, predecessor):
        self.predecessor = predecessor

    def get_successor(self):
        return self.successor

    def get_predecessor(self):
        return self.predecessor

    def get_value(self):
        return self.value

    def get_obj(self):
        return self.obj

class PrioQueue:
    
    def __init__(self):
        # elements should be elements
        self.first_elem = None
        self.last_elem = None

    def __str__(self):
        if self.empty():
            return "PrioQueue: []"
        out = ""
        curr = self.first_elem
        while curr != self.last_elem:
            out = out + str(curr) + "\n"
            curr = curr.get_successor()
        out = out + str(curr)
        return "PrioQueue: {}".format(out)

    def __repr__(self):
        return self.__str__()

    def add(self, obj, weight):
        elem = Element(obj, weight)
        if self.empty():
            self.first_elem = elem
            self.last_elem = elem
        else:
            # last element
            if elem.get_value() >= self.last_elem.get_value():
                self.last_elem.set_successor(elem)
                elem.set_predecessor(self.last_elem)
                self.last_elem = elem
                return
            # first element
            elif elem.get_value() < self.first_elem.get_value():
                self.first_elem.set_predecessor(elem)
                elem.set_successor(self.first_elem)
                self.first_elem = elem 
                return
            curr = self.first_elem
            while curr.get_value() < elem.get_value():
                curr = curr.get_successor()
            # assert curr.get_val >= elem.get_val
            elem.set_predecessor(curr.get_predecessor())
            elem.set_successor(curr)
            curr.set_predecessor(elem)
    
    def reset(self):
        self.last_elem = None
        self.first_elem = None

    def get_max(self):
        if self.empty():
            return None
        maxi = self.last_elem
        if self.last_elem == self.first_elem:
            self.reset()
        else:
            self.last_elem = self.last_elem.get_predecessor()
            self.last_elem.set_successor(None)
        return maxi.get_obj()
    
    def get_min(self):
        if self.empty():
            return None
        mini = self.first_elem
        if self.first_elem == self.last_elem:
            self.reset()
        else:
            self.first_elem = self.first_elem.get_successor()
            self.first_elem.set_predecessor(None)
        return mini.get_obj()
    
    def empty(self):
        return self.first_elem is None

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

    def calculate_distances_to(self, vert):
        distance = {}
        visited = {}
        for vertex in self.vertices:
            distance[vertex] = -1
            visited[vertex] = False
        distance[vert] = 0
        
        unvisited = PrioQueue()
        unvisited.add(vert, 0)
        while not unvisited.empty():
            curr = unvisited.get_min()
            visited[curr] = True
            edges = curr.get_incoming_edges()
            for edge in edges:
                neighbor = edge.get_source()
                if not visited[neighbor]: 
                    # update distances
                    newdist = distance[curr] + edge.get_weight()
                    if distance[neighbor] == -1 or newdist < distance[neighbor]:
                        distance[neighbor] = newdist
                    # update unvisited
                    unvisited.add(neighbor, distance[neighbor])
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
    g = Graph(filename, True)

    vert = g.get_vertex_by_name(vert)
    res = g.get_longest_shortest_to(vert)
    
    print("RESULT VERTEX {}\n\nRESULT DIST {}".format(res[0].get_name(), res[1]))
