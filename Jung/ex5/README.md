This is a program which uses dijkstra algorithm to computes the longest shortes path in a graph

How to Build: 
enter "make build" in your terminal

How to run:
enter "./Ex5 filename -m1" for the computation with the boost dijkstra algorithm and "/.Ex5 filename -m2" for my own implementation of the dijkstra algorithm


My own algorithm needs significantly more time for the bigger graphs ,for example on my laptop it takes appr. 100 seconds for acycpos-1.gph ( found on http://www.zib.de/koch/SP/data/) while the boost algorithm just needs 2.5 seconds
My impementation with the boost algo gets a segmentation fault, I still need to solve this problem
