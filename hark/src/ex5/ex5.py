# coding=utf-8
import sys
import math
import networkx as nx
import numpy as np
import timeit


## @package ex5
# This script is a solution for exercise 5 from the advanced programming course in the sommer 2017 at TUB.
# For more information see @link http://www.zib.de/koch/lectures/ss2017_appfs.php

## This method validates a line of an input .gph=file
# @param line_as_array: line split into array at each ";".
# @param line_number: current line number for the case of error printing including the line number
# @param stripped: line split into array at each ";" and strip (removed white spaces) afterwards.
# @return: False if not all of the expected 3 strings can be parsed to an integer.
def is_valid_line_of_gph_file(line_as_array, line_number, stripped):
    try:
        node1 = int(line_as_array[0])
        node2 = int(line_as_array[1])
        arc_weight = int(line_as_array[2])
        if math.isnan(node1) or math.isnan(node2) or math.isnan(arc_weight):
            print("Line {} : Invalid value (NaN)".format(line_number + 1))
            return False
        if arc_weight > 2000000000 or arc_weight <= 0:
            print("Line {} : weight of arc is greater than specified".format(line_number + 1, arc_weight))
            return False
    except ValueError:
        print("INFORM Line {} : Syntax error [{}]".format(line_number + 1, stripped))
        return False
    return node1, node2, arc_weight


## This method is the routine in case of file-input
# @param filename: path of the .gph input-file.
# @return: the specified result output
def import_gph_from_file(filename):
    maximum_arc_weight = 0
    g = nx.Graph()
    with open(filename, "r") as file:
        for line_count, line in enumerate(file):
            if line_count == 0:
                continue
            stripped_line = line.strip()
            if stripped_line == "":
                continue
            split_line = stripped_line.split(" ")

            if len(split_line) != 3:
                print("INFORM Line {} : Syntax error [{}]".format(line_count + 1, split_line))
                continue

            if is_valid_line_of_gph_file(split_line, line_count, stripped_line) is not False:
                node_id1, node_id2, arc_weight = is_valid_line_of_gph_file(split_line, line_count, stripped_line)
                if arc_weight > maximum_arc_weight:
                    maximum_arc_weight = arc_weight
                # print('adding: (', node_id1, ', ', node_id2, ') ', arc_weight)
                g.add_edge(node_id1, node_id2, weight=arc_weight)
        print 'INFORM added ', g.number_of_nodes(), 'nodes and ', g.number_of_edges(), 'edges'
        preds, dists = nx.bellman_ford(g, 1)
        shortest_distances = np.array(dists.items(), dtype=np.uint32)
        node_maximal_shortest_path_dist = np.max(shortest_distances.transpose()[1])
        node_maximal_shortest_path_id = np.max(
            shortest_distances.transpose()[0, np.argmax(shortest_distances.transpose()[1])])
        # as in the references of networkX-lib the running time of bellman-algo is in O(m*n).
        # every node needs to be touched in order to check if there is a (shortest) path to node 1.
        rating = 1
    return node_maximal_shortest_path_id, node_maximal_shortest_path_dist, maximum_arc_weight, rating


## This method is the routine in case the user wants to input the graph through <stdin>
# @return: the specified result output
def import_gph_from_stdin():
    maximum_arc_weight = 0
    g = nx.Graph()
    wait_for_first_line = True
    while True:
        input_string = sys.stdin.readline()
        if input_string == '\n':
            break
        else:
            try:
                stripped_line = input_string.strip()
                if stripped_line == "":
                    print("INFORM Syntax error [{}]".format(stripped_line))
                    continue
                split_line = stripped_line.split(" ")
                if len(split_line) != 3:
                    if wait_for_first_line:
                        continue
                    else:
                        print("INFORM Syntax error [{}]".format(split_line))
                node_id1 = int(split_line[0])
                node_id2 = int(split_line[1])
                arc_weight = int(split_line[2])
                if math.isnan(node_id1) or math.isnan(node_id2) or math.isnan(arc_weight):
                    print("INFORM Invalid value (NaN)")
                    continue
                if arc_weight > 2000000000 or arc_weight <= 0:
                    print("INFORM weight of arc is greater than specified")
                    continue
                wait_for_first_line = False
                if arc_weight > maximum_arc_weight:
                    maximum_arc_weight = arc_weight
                g.add_edge(node_id1, node_id2, weight=arc_weight)
            except ValueError:
                print("INFORM Syntax error [{}]".format(stripped_line))
    print 'INFORM added ', g.number_of_nodes(), 'nodes and ', g.number_of_edges(), 'edges'
    preds, dists = nx.bellman_ford(g, 1)
    shortest_distances = np.array(dists.items(), dtype=np.uint32)
    node_maximal_shortest_path_dist = np.max(shortest_distances.transpose()[1])
    node_maximal_shortest_path_id = np.max(
        shortest_distances.transpose()[0, np.argmax(shortest_distances.transpose()[1])])
    # as in the references of networkX-lib the running time of bellman-algo is in O(m*n).
    # every node needs to be touched in order to check if there is a (shortest) path to node 1.
    rating = 1
    return node_maximal_shortest_path_id, node_maximal_shortest_path_dist, maximum_arc_weight, rating


start_time = timeit.default_timer()
if len(sys.argv) != 2:
    print("INFORM wrong number of program arguments")
else:
    if sys.argv[1] == "-":
        print("INFORM try to read from <stdin>")
        node, dist, marc, rati = import_gph_from_stdin()
    else:
        print("INFORM try to read from file: " + sys.argv[1])
        node, dist, marc, rati = import_gph_from_file(sys.argv[1])
    print 'RESULT NODE ', node
    print 'RESULT DIST ', dist
    print 'RESULT MARC ', marc
    print 'RESULT TIME ', timeit.default_timer() - start_time
    print 'RESULT RATI ', rati
