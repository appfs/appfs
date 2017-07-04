# Evercise 8: Steiner Tree Heuristic

Consider the input graphs from exercise 6/7 as [Steiner tree problems in graphs](https://en.wikipedia.org/wiki/Steiner_tree_problem) by considering
each vertex with a prime index (i.e., with vertex number 2,3,5,7...) as a terminal.

***Implement the so called shortest paths heuristic***:
Starting with a single terminal, the heuristic iteratively connects the current subtree to a nearest
terminal by a shortest path. This procedure is reiterated until all terminals are spanned. A straightforward approach to implement
it is to compute for each terminal the shortest paths to all other vertices. Then you need to look in each iteration of the heuristic 
for the terminal that is closest to your current subtree and connect it.
Your program should ***print the objective value*** of the solution it has found.
