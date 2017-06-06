#!/usr/bin/env python 

import sys
from graph_tool.all import Graph, load_graph_from_csv
from queue import *

class Ex5:

    def __init__(self, file, directed=False, vertex=1):
        self.g = load_graph_from_csv(filename, directed=directed, csv_options={"delimiter": " ", "quotechar": '"'}, skip_first=True, string_vals=True)
        self.vertex_names = self.g.vertex_properties["name"]

    def calculate_distances_to(self, vert):
        distance = {}
        visited = {}
        for vertex in range(self.g.num_vertices()):
            distance[vertex] = -1
            visited[vertex] = False
        distance[vert] = 0
       
        weights = self.g.edge_properties["c0"]
        unvisited = PriorityQueue(self.g.num_vertices())
        unvisited.put((0, vert))
        while not unvisited.empty():
            curr = unvisited.get()[1]
            visited[curr] = True
            edges = self.g.get_in_edges(curr)
            for (s,t,e) in edges:
                neighbor = s
                if not visited[neighbor]: 
                    # update distances
                    newdist = distance[curr] + int(weights[self.g.edge(s,t)])
                    if distance[neighbor] == -1 or newdist < distance[neighbor]:
                        distance[neighbor] = newdist
                    # update unvisited
                    unvisited.put((distance[neighbor], neighbor))
        return distance
   
    def vert_index(self, vert):
        for v in range(self.g.num_vertices()):
            if int(self.vertex_names[v]) == vert:
                return v
   
    def vert_name(self, vert):
        return self.vertex_names[vert]

    def get_longest_shortest_to(self, vert):
        # name to index
        vert = self.vert_index(vert)
        distances = self.calculate_distances_to(vert)
        max_dist = 0
        max_vert = vert
        for v in range(self.g.num_vertices()):
            dist = distances[v]
            if dist > max_dist or (dist == max_dist and self.vert_name(max_vert) > self.vert_name(v)):
                max_dist = dist
                max_vert = v
        return (self.vert_name(max_vert), max_dist)

if __name__ == "__main__":
    # read filename from commandlineargs
    filename = sys.argv[1]
    #vert = int(sys.argv[2])
    vert = 1
    ex = Ex5(file=filename, directed=True, vertex=vert)
    res = ex.get_longest_shortest_to(vert)
    
    print("RESULT VERTEX {}\n\nRESULT DIST {}".format(res[0], res[1]))
