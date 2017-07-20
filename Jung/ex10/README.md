Program for Steiner Tree Problem Heuristic, i use the primes from primes.utm.edu

compile: run "make" in your console, 
for bigger graphs run "make BIG" (with more than 100000 vertexes, you can run this also on smaller graphs but it will take more time)

run: run "make THREADS=XX FILE=file.gph test" or "make THREADS=XX STARTS=YY FILE=file.gph test" 
for the algorithm with XX threads and the best solution from the first YY start points
for bigger graphs run "make THREADS=XX STARTS=YY FILE=file.gph testBIG"

you can also run it with /.ex10 file.gph YY XX  , 
where int YY is the number of the start-terminals to test (default=100) and int XX the number of threads to use(default=1)
