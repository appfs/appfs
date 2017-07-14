# EXERCISE 5: Graphs and shortest paths

Write a program that:

* reads in a graph from a file given in .gph format (see [here](http://www.zib.de/koch/SP/data/) for examples) with the filename provided as a command line argument. Note that the graphs have positive edge weights (that are always below 2000000000).

* computes a longest (with respect to the edge weights) shortest path from any vertex to the vertex with index 1. In case of ties the vertex with smallest index should be chosen.

* gives an output with the following syntax:

    RESULT VERTEX 42

    RESULT DIST 5553



You may use graph libraries such as boost (for C++) or graph-tool (for Python), but you are not allowed to copy the entire program.



**Deadline: June 8, 18:00**
