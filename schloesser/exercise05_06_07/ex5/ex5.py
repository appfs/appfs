#!/usr/bin/env python3

'''
Exercises 5, 6 and 7

This package provides means to solving exercises 5 to 7 from the lecture appfs.
'''
import sys
import time

if __name__ == "__main__":
    T0CLOCK = time.perf_counter()
    T0TIME = time.process_time()

    # read filename from commandlineargs
    if len(sys.argv) < 2:
        print("Please provide a file to be read from.")
        exit()
    MODE = sys.argv[1]
    VERT = 1

    if not MODE in ["-m1", "-m2"]:
        INFILE = MODE
        MODE = "-m2"
    else:
        if len(sys.argv) < 3:
            print("Please provide a file to be read from.")
            exit()
        INFILE = sys.argv[2]

    if MODE == "-m2":
        try:
            from GraphWrapper import GraphWrapper
            GRAPH = GraphWrapper(filename=INFILE, directed=False)
        except ImportError:
            print("For this you need to have a running version of graph-tool installed.\nSwitching back to mode m1.")
            MODE = "-m1"

    if MODE == "-m1":
        from MyGraph import Graph
        GRAPH = Graph(INFILE, False)

    RES = GRAPH.get_longest_shortest_to(VERT)

    print("RESULT VERTEX {}\nRESULT DIST {}".format(RES[0], RES[1]))

    T1CLOCK = time.perf_counter()
    T1TIME = time.process_time()

#    print("Program took {} s (wallclock time)".format(T1CLOCK - T0CLOCK))
    print("RESULT TIME {}".format(T1TIME - T0TIME))
