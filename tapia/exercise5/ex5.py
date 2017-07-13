import sys

import networkx as nx


def is_valid(line):
    """
    Checks if the content of an edge has a valid format.
    <vertex vertex weight>

    :param line: A line of the input text.
    :type: String
    :return: A list if edge is valid, None otherwise.
    """
    edge = line.rsplit()
    wrong_args_number = len(edge) != 3
    is_comment = line.startswith("#")

    if wrong_args_number or not edge or is_comment:
        return None
    try:
        int(edge[0])
        int(edge[1])
        int(edge[2])
    except ValueError:
        return None
    return edge


def generate_edges(content):
    """
    Yields an edge <vertex vertex weight> to the graph if it's valid.

    :param content: The raw content of the input file.
    """

    for edge in content:
        valid_edge = is_valid(edge)
        if valid_edge:
            yield valid_edge


def load_graph_data(content):
    """
    Loads the content of the input file as a graph and returns
    a graph structure and its first node.

    :param content: The raw content of the input file.
    :return: An undirected Graph and a Node.
    """
    G = nx.Graph()
    edges = generate_edges(content)
    for edge in edges:
        if edge:
            G.add_edge(edge[0], edge[1], weight=edge[2])
    return G


def dijkstra(graph):
    """
    Shortest path algorithm. Returns a dictionary with the shortest distance from each
    node in the graph to the initial node '1'.

    :param graph: A Graph structure
    :return: Dictionary with <node: distance> where the distance is the shortest path to the initial node.
    """
    nodes = graph.nodes()
    initial_node_index = nodes.index('1')
    initial_node = nodes[initial_node_index]

    neighbors_initial_node = graph.neighbors(initial_node)

    distances = dict()
    for node in nodes:
        if node in neighbors_initial_node:
            distances[node] = int(graph[initial_node][node]['weight'])
        else:
            distances[node] = 2000000000

    distances[initial_node] = 0
    visited = [initial_node]

    while sorted(nodes) != sorted(visited):
        current_node = min_vertex(distances, visited)
        visited.append(current_node)

        for neighbor in graph.neighbors(current_node):
            distance_to_neighbor = int(graph[current_node][neighbor]['weight'])
            temp_dist = distances[current_node] + distance_to_neighbor

            if temp_dist < distances[neighbor]:
                distances[neighbor] = temp_dist

    return distances


def min_vertex(distances, visited):
    '''
    Returns the vector with the minimum distance which is not yet visited.

    :param distances: A dictionary with <node: distance> structure
    :param visited: A list with the visited nodes
    :return: The non-visited vertex with the minimum distance
    '''
    min_distance = 2000000000
    min_vertex = None

    for node, neighbor_distance in distances.items():
        if node in visited:
            continue

        if neighbor_distance < min_distance:
            min_distance = neighbor_distance
            min_vertex = node

    return min_vertex

if __name__ == '__main__':
    filename = sys.argv[-1]
    content = open(filename)

    graph = load_graph_data(content)

    distances = dijkstra(graph)

    result_dist = 2000000000
    result_vertex = None

    for k, v in distances.items():

        if k == '1':
            continue

        if v < result_dist:
            result_dist = v
            result_vertex = k

        elif v == result_dist:
            key_list = list(distances.keys())
            index_result = key_list.index(result_vertex)
            index_current = key_list.index(k)
            if index_result < index_current:
                result_vertex = k

    print('RESULT VERTEX {result_vertex}'.format(result_vertex=result_vertex))
    print('RESULT DIST {result_dist}'.format(result_dist=result_dist))
