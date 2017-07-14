import sys

import time


class Graph(object):
    """
    Defines an undirected graph with dictionary structure {}
    """
    def __init__(self):

        self._edges = dict()
        self._nodes = list()

    def _add_node(self, node):
        """
        Adds a node to the list of nodes in a Graph.

        :type node: String
        """
        if node not in self._nodes:
            self._nodes.append(node)
        else:
            print("Duplicated Node, skipping.")

    def nodes(self):
        """
        Returns the nodes in a Graph
        """
        return self._nodes

    def edges(self):
        """
        Returns the edges in a Graph
        """
        return list(self._edges.keys())

    def add_edge(self, src_node, dst_node, **properties):
        """
        Adds an edge to the Graph content dictionary.
        Edge structure (src_node, dst_node): {property: value}

        :type src_node: String
        :type dst_node: String
        :param properties: Edge labels.
        :type properties: dict
        """
        if src_node not in self._nodes:
            self._add_node(src_node)
        if dst_node not in self._nodes:
            self._add_node(dst_node)
        if src_node == dst_node:
            print("Self loops not supported.")
            return

        coordinate_right = (src_node, dst_node)
        coordinate_left = (dst_node, src_node)
        edges = self.edges()
        if (coordinate_right not in edges) and (coordinate_left not in edges):
            self._edges[coordinate_right] = properties
        else:
            print("Duplicated edge, skipping.")

    def get_edge(self, src_node, dst_node):
        """
        Returns an edge value if the edge exist in the Graph.

        :type src_node: String
        :type dst_node: String
        :return: a dict with the properties for the given edge
        """
        coordinate_right = (src_node, dst_node)
        coordinate_left = (dst_node, src_node)
        edges = self.edges()

        if coordinate_right in edges:
            return self._edges[coordinate_right]
        if coordinate_left in edges:
            return self._edges[coordinate_left]
        else:
            return {}

    def show(self):
        for k, v in self._edges.items():
            print("{coordinate}: weight={weight}".format(coordinate=k, weight=v['weight']))

    def neighbors(self, node):
        """
        Returns the neighbors of a node.

        :type node: String
        :return: A list with the neighbors of a node
        """
        neighbors = list()
        for k, v in self._edges.items():
            if node in k:
                if node == k[0]:
                    neighbors.append(k[1])
                else:
                    neighbors.append(k[0])
        return neighbors


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
    G = Graph()
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
            distances[node] = int(graph.get_edge(initial_node, node)['weight'])
        else:
            distances[node] = 2000000000

    distances[initial_node] = 0
    visited = [initial_node]

    while sorted(nodes) != sorted(visited):
        current_node = min_vertex(distances, visited)
        visited.append(current_node)

        for neighbor in graph.neighbors(current_node):
            distance_to_neighbor = int(graph.get_edge(current_node, neighbor)['weight'])
            temp_dist = distances[current_node] + distance_to_neighbor

            if temp_dist < distances[neighbor]:
                distances[neighbor] = temp_dist

    return distances


def min_vertex(distances, visited):
    """
    Returns the vector with the minimum distance which is not yet visited.

    :param distances: A dictionary with <node: distance> structure
    :param visited: A list with the visited nodes
    :return: The non-visited vertex with the minimum distance
    """
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
    start = time.time()  # wall-clock-time start

    filename = sys.argv[-1]
    content = open(filename)

    graph = load_graph_data(content)

    distances = dijkstra(graph)

    result_dist = 0
    result_vertex = None

    for k, v in distances.items():

        if k == '1':
            continue

        if v > result_dist:
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

    end = time.time()  # wall-clock-time end
    wall_clock_time = end - start
    print('WALL-CLOCK TIME: {0:.2f} seconds'.format(wall_clock_time))

    user_time = time.process_time()
    print('USER TIME: {0:.2f} seconds'.format(user_time))
