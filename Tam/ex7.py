# APPFS Exercise 7 aka 5b
# by Tam Tran
# in Python3

# 3 goals:
# 1) read in graph as CSV and extract its edge information to distances between vertices
# 2) update the distance between the vertices anytime a shorter distance is found (shortest path)
# 3) return the longest shortest path

import sys
from collections import defaultdict, OrderedDict
from itertools import chain
import time
from resource import getrusage as resource_usage, RUSAGE_SELF

def run(graphfile):
    
    # generate 'graph' dictionary
    d1, d2, graph = defaultdict(list), defaultdict(list), defaultdict(list)
    with open(graphfile) as f:
        next(f) # skip first row
        for line in f:
            a,b,c = line.strip().split()
            d1[int(a)].append((int(b),float(c)))
            d2[int(b)].append((int(a),float(c)))

    for k,v in chain(d1.items(), d2.items()):
        graph[k].extend(v)

    # generate 'distances' dictionary
    distances = defaultdict(list)
    for k,_ in graph.items():
        distances[k] = (float('inf'), None)

    # setting the vertex 1 as our reference/starting vertex
    unvisited = list(distances.keys())
    s = 1
    unvisited.remove(s)
    distances[s] = (0, None)
    next_s = s

    # run shortest path algorithm
    while unvisited:
        distances, next_s = update_distances(graph, distances, next_s, unvisited)
        unvisited.remove(next_s)

    assert (not(set(list(distances.keys())) & set(unvisited))), "nicht gucci"
        
    # get longest shortest path
    shortest_distance = lambda item: item[1][0]
    vertex_furthest = list(OrderedDict(sorted(distances.items(), key=shortest_distance)).keys())[-1]
    distance_furthest = distances[vertex_furthest][0]
    print("RESULT VERTEX", vertex_furthest)
    print("RESULT DIST", int(distance_furthest))


    
def update_distances(graph, distances, s, unvisited):
    
    s_neighbors = graph[s]
    for (dnode, edge_weight) in s_neighbors:
        
        current_dist = distances[dnode][0] # tuple index 0 corresponds to current shortest distance
        s_weight = distances[s][0]
        compare_dist = s_weight + edge_weight
        if current_dist > compare_dist:
            distances[dnode] = (compare_dist, s)

    # get the next_s that is still in unvisited based on min shortest distance
    shortest_distance = lambda item: item[1][0]
    distances_sortedkeys = OrderedDict(sorted(distances.items(), key=shortest_distance)).keys()
    for potential_s in list(distances_sortedkeys):
        if potential_s in unvisited:
            next_s = potential_s
            break

    return distances, next_s



graphfile = sys.argv[1]
t0, s0 = time.time(), resource_usage(RUSAGE_SELF)
run(graphfile)
s1, t1 = resource_usage(RUSAGE_SELF), time.time()
print("wall clock time", t1 - t0)
print("user clock time", s1.ru_stime - s0.ru_stime)


# notes:
# for searching through dataframe rows meeting condition, df.columnname.isin([columnvaluefilter]) is faster than df[columnname] = columnvaluefilter
# but what is overall faster than pandas dataframes for accessing values is the dictionary (bc values already sorted and there are more libraries to support dicts)

