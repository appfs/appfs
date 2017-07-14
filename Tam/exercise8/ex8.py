# APPFS Ex8
# by Tam Tran

'''This program will generate the steiner tree, which is the minimum subgraph of G spanning all the terminal nodes'''

import sys
import networkx as nx
from sympy import sieve
import time
from resource import getrusage as resource_usage, RUSAGE_SELF
import heapq

def run(graphfile):
    '''reads a graph file and prints stats on the steiner subgraph generated'''
    # generate graph from file
    G = nx.read_edgelist(graphfile, nodetype=int, data=(('weight',float),))

    for u,v in G.edges():
        if not G[u][v]: #removing edges w/o a weight
            G.remove_edge(u,v) # the nodes will still remain tho
            if G.neighbors(u) == []:
                G.remove_node(u)
            if G.neighbors(v) == []:
                G.remove_node(v)

    # set terminal nodes
    V = list(G.nodes())
    n = len(V)
    if set(V) == set(list(range(1,n+1))):
        T = list(sieve.primerange(1, n+1))
    else:
        T = []
        for v in V:
            if v in list(sieve.primerange(v, v+1)):
                T.append(v)
    
    # generate steiner tree
    steinerG = gen_steiner(G,T)
    print('original G -> no of edges:', G.size(), ', sum of edge weights', G.size(weight='weight'))
    print('steinerG -> no of edges:', steinerG.size(), '(', format(100*steinerG.size()/float(G.size()), '.2f'), '%)', \
          ', sum of edge weights', steinerG.size(weight='weight'), '(', format(100*steinerG.size(weight='weight')/float(G.size(weight='weight')), '.2f'), '%)')
    print('steinerG is connected:', nx.is_connected(steinerG))
    print('steinerG is acyclic:', not bool(nx.cycle_basis(steinerG)))



def gen_steiner(G,T):
    '''generates steiner subgraph based on graph G and list of terminal nodes T, \
        based on m2: fixed loneT x all steinerNodes, with heapq instead of min()'''
    
    # generate steiner tree
    steinerG = nx.Graph()
    steinerG.add_node(T[0]) # set root node
    loneTerminals = list(set(T) - set(steinerG.nodes()))
    
    while loneTerminals:
        
        # just selecting an arbitrary loneT since they will all get added to steinerG anyways
        # disclaimer: but then the edges that connect the Tnodes will differ
        # loneT = loneTerminals[0] -> b15.gph 215; loneT = loneTerminals[-1] -> b15.gph 211
        loneT = loneTerminals[-1]
        
        # this is equivalent to a Dijkstra of loneT as root, to the nodes of steinerG
        loneDistances = [] #unique for each loneT, hence it's ok to be generated each time
        for steinerNode in steinerG.nodes():
            length = nx.shortest_path_length(G, source=steinerNode, target=loneT, weight='weight')
            heapq.heappush(loneDistances, (steinerNode, loneT, length))
        mSrc,mDst, mLength = heapq.nsmallest(1, loneDistances, key=lambda x: (x[2], x[0]))[0] #bc it's a list

        # add path of steinerG to loneT to steinerG
        new_path = nx.shortest_path(G, mSrc, mDst, weight='weight') # mDst should just be loneT
        for u,v in zip(new_path[:-1],new_path[1:]): # parse through list to obtain weight info
            steinerG.add_edge(u,v, weight=G[u][v]['weight'])
        
        loneTerminals.remove(mDst)
            
    return steinerG


t0, s0 = time.time(), resource_usage(RUSAGE_SELF)
run(sys.argv[1])
s1, t1 = resource_usage(RUSAGE_SELF), time.time()
print("wall clock time", t1 - t0)
print("user clock time", s1.ru_stime - s0.ru_stime)
