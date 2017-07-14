#!/usr/bin/env python3

'''
helper functions
'''

def pop_next_destination(to_visit, distance):
    '''
    Return a vertex that still has to be visited and has currently smallest distance to startvertex.

    Args:
        to_visit (array): an array of bools indicating whether or not a vertex has to be visited.
        distance (array): an array containing the current shortest distances to the corresponding vertices.
    '''
    mind = -1
    minv = None
    for vert in to_visit:
        # assert distance[v] >= 0
        if distance[vert] < mind or mind == -1:
            mind = distance[vert]
            minv = vert
    to_visit.remove(minv)
    return minv
