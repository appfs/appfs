# APPFS Exercise 5
# by Tam Tran
# in Python3

# 3 goals:
# 1) read in graph as CSV and extract its edge information to distances between vertices
# 2) update the distance between the vertices anytime a shorter distance is found (shortest path)
# 3) return the longest shortest path

import sys
import pandas as pd
import numpy as np
import time
from resource import getrusage as resource_usage, RUSAGE_SELF

def run(graphfile):
    
    graph = pd.read_csv(graphfile, sep = " ", names = ['src', 'dst', 'weight']).drop(0)
    
    # generate distances dataframe
    unique_vertices = np.unique(np.append(graph['src'].unique(), graph['dst'].unique()))
    distances = pd.DataFrame(unique_vertices, columns = ['vertex'])
    distances['shortest_distance'] = [float('inf')] * distances.shape[0]
    distances['previous_vertex'] = [None] * distances.shape[0]
    
    # to keep track of the vertices that have been processed
    visited = []
    unvisited = list(distances['vertex'])
    
    # setting the vertex 1 as our reference/starting vertex
    s = 1
    unvisited.remove(s)
    visited.append(s)
    distances.loc[distances.vertex == s, 'shortest_distance'] = 0 
    next_s = s

    # run shortest path algorithm
    while unvisited:
        distances, next_s = update_distances(graph, distances, next_s, unvisited)
        unvisited.remove(next_s)
        visited.append(next_s)
        
    # get longest shortest path
    print("RESULT VERTEX", distances.loc[distances['shortest_distance'].argmax(), 'vertex'])
    print("RESULT DIST", int(distances['shortest_distance'].max()))
    
    
def update_distances(graph, distances, s, unvisited):
    print('s', s)
    s_neighbors = list(graph[graph['src'] == s]['dst']) + list(graph[graph['dst'] == s]['src'])
    
    for dnode in s_neighbors:
        
        current_dist = distances[distances.vertex == dnode]['shortest_distance'].iloc[0]
        s_weight = distances.loc[distances.vertex == s, 'shortest_distance']
        
        edge_weight1 = graph.loc[(graph['src'] == s) & (graph['dst'] == dnode), 'weight']
        edge_weight2 = graph.loc[(graph['dst'] == s) & (graph['src'] == dnode), 'weight']
        
        if not edge_weight1.empty: edge_weight = edge_weight1.iloc[0]
        elif not edge_weight2.empty: edge_weight = edge_weight2.iloc[0]
        
        compare_dist = float(s_weight) + float(edge_weight)
        
        if current_dist > compare_dist:
            distances.loc[distances.vertex == dnode, ['shortest_distance', 'previous_vertex']] = compare_dist, s

    # get the next_s that is still in unvisited
    indices = distances['shortest_distance'].argsort() #returns list of indices of elements in ascending order
    for i in indices:
        if distances.iloc[i]['vertex'] in unvisited:
            next_s = distances.iloc[i]['vertex']
            break

    return distances, next_s    


graphfile = sys.argv[1]
t0, s0 = time.time(), resource_usage(RUSAGE_SELF)
run(graphfile)
s1, t1 = resource_usage(RUSAGE_SELF), time.time()
print("wall clock time", t1 - t0)
print("user clock time", s1.ru_stime - s0.ru_stime)
