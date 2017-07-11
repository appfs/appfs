# Exercise 9: Improved Steiner Tree Heuristic

Implement a modified version of the shortest path heuristic described in the previous exercise:
Instead of computing shortest paths from each terminal to all other vertices, start Dijkstra's algorithm from a terminal and 
whenever a new terminal is scanned (i.e. removed from the priority queue) insert all vertices on the (shortest) subpath between this
terminal and your current subtree into you priority queue (with key 0.0). Note that this heuristic has the same worst-case complexity as the original one, but is empirically almost ever faster.

Finally, start the heuristic from several terminals and report the best result.
