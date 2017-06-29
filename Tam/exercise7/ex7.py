# APPFS Exercise 7 aka 5b
# by Tam Tran
# in Python3

# goals:
# 2 modes of calculating the longest shortest path, library vs self written

""" This program will calculate the longest shortest path of a graph from the vertex 1 and it will use either the library-given method or the self-written method as specified """

import sys
from collections import defaultdict, OrderedDict
from itertools import chain
import time
from resource import getrusage as resource_usage, RUSAGE_SELF
import networkx as nx
import operator

def run(mode, graphfile):
    """Executes the program on the given input arguments
        
        Args:
            mode: indicate which method to execute program with
            graphfile: the graph to traverse
        
        Returns: print statement indicating the furthest vertex from Vertex 1 and its respective distance
        
        Raises: ValueError and AssertionError
    """
    
    if mode == '-m1': noLibrary(graphfile)
    elif mode == '-m2': yesLibrary(graphfile)
    else: print('please specify either \'-m1\' or \'-m2\'')


def yesLibrary(graphfile):
    """Calculates the longest shortest path using the python package NetworkX. With respect to this program, this function is invoked in the run() function.
        
        Args:
            graphfile: the graph to traverse
    """

    # generate graph using only edges w/ weights
    G = nx.read_edgelist(graphfile, nodetype=int, data=(('weight',float),))

    for u,v in G.edges():
        if not G[u][v]:
            G.remove_edge(u,v)

    # returns dict of lengths by shortest path
    length=nx.single_source_dijkstra_path_length(G,1) # dict {node: shortest_distance}

    # get longest shortest path
    vertex_furthest = max(length.items(), key=operator.itemgetter(1))[0]
    print("RESULT VERTEX", vertex_furthest)
    print("RESULT DIST", int(length[vertex_furthest]))


def noLibrary(graphfile):
    """Calculates the longest shortest path using self-written Djisktra's algorithm and dictionary data objects. With respect to this program, this function is invoked in the run() function.
        
        Args:
            graphfile: the graph to traverse
    """
    
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
    """Per iteration of evaluating the distances of paths from a single node, this function updates the data object keeping track of such distances. With respect to this program, this function is invoked in the noLibrary() function.
        
        Args:
            graph: a dictionary data object containing information of the graph's vertices and edges
            distances: a dictionary data object containing the current vertex distances from Vertex 1 and its preceeding vertix for the path from Vertex 1
            s: an index for the current vertex being traversed
            unvisited: a list of the vertices of graph that have not yet been traversed
            
        Returns:
            distances: an updated data object
            next_s: the next vertex to traverse
    """
    
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


t0, s0 = time.time(), resource_usage(RUSAGE_SELF)
run(sys.argv[1], sys.argv[2])
s1, t1 = resource_usage(RUSAGE_SELF), time.time()
print("wall clock time", t1 - t0)
print("user clock time", s1.ru_stime - s0.ru_stime)


# notes:
# for searching through dataframe rows meeting condition, df.columnname.isin([columnvaluefilter]) is faster than df[columnname] = columnvaluefilter
# but what is overall faster than pandas dataframes for accessing values is the dictionary (bc values already sorted and there are more libraries to support dicts)

