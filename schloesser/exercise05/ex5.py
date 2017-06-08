#!/usr/bin/env python 

import sys
from graph_tool.all import Graph, load_graph_from_csv
from queue import *

class Ex5:

    def __init__(self, file, directed=False, vertex=1):
        self.g = load_graph_from_csv(filename, directed=directed, csv_options={"delimiter": " ", "quotechar": '"'}, skip_first=True, string_vals=True)
        self.vertex_names = self.g.vertex_properties["name"]

    def pop_next_destination(self, to_visit, distance):
        mind = -1
        minv = None
        for v in to_visit:
            if distance[v] < mind or mind == -1: 
                mind = distance[v]
                minv = v
        to_visit.remove(minv)
        return minv

    def calculate_distances_to(self, vert):
        directed = self.g.is_directed()
        distance = {}
        visited = {}
        for vertex in self.g.get_vertices():
            distance[vertex] = -1
            visited[vertex] = False
        distance[vert] = 0
        to_visit = set()
       
        weights = self.g.edge_properties["c0"]
        to_visit.add(vert)
        while len(to_visit) != 0:
            curr = self.pop_next_destination(to_visit, distance)
            visited[curr] = True
            if directed:
                edges = self.g.get_in_edges(curr)
            else:
                edges = self.g.get_out_edges(curr)
            for (s,t,e) in edges:
                if directed:
                    neighbor = s
                else: 
                    neighbor = t
                if not visited[neighbor]: 
                    # update distances
                    newdist = distance[curr] + int(weights[self.g.edge(s,t)])
                    if distance[neighbor] == -1 or newdist < distance[neighbor]:
                        distance[neighbor] = newdist
                    to_visit.add(neighbor)
        return distance
   
    def vert_index(self, vert):
        for v in range(self.g.num_vertices()):
            if int(self.vertex_names[v]) == vert:
                return v
   
    def vert_name(self, vert):
        return int(self.vertex_names[vert])

    def get_longest_shortest_to(self, vert):
        # name to index
        vert = self.vert_index(vert)
        distances = self.calculate_distances_to(vert)
        max_dist = 0
        max_vert = vert
        for v in self.g.get_vertices():
            dist = distances[v]
            if dist > max_dist or (dist == max_dist and self.vert_name(v) < self.vert_name(max_vert)):
                max_dist = dist
                max_vert = v
        return (self.vert_name(max_vert), max_dist)

if __name__ == "__main__":
    # read filename from commandlineargs
    filename = sys.argv[1]
    #vert = int(sys.argv[2])
    
    vert = 1
    ex = Ex5(file=filename, directed=False, vertex=vert)
    res = ex.get_longest_shortest_to(vert)
    
    print("RESULT VERTEX {}\n\nRESULT DIST {}".format(res[0], res[1]))
