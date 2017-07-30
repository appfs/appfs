This is my program for the steiner tree Heuristic from Takahashi, Matsuyama

How to compile: run make in your console

How to run: run 'make FILE=X STARTS=Y THREADS=Z test' where STARTS are the number of terminals, 
        or you just run ./ex10 <file.gph> <STARTS> <THREADS> (-s) 
  
  
  note:
  if you run with the file phard-x.gph you maybe need to unlimit your stack ( ulimit -s unlimited),
  since the depth search recursion i use when i check if its a steiner tree leads to a stack overflow, 
  unfortunenately i discovered that too late to fix it in a better way. 
  
  if you run 'make doc' you need to set in EXTRACT_ALL=YES in the Doxyfile and run 'doxygen Doxyfile' 
  
  if you run the program with valgrind with multiple threads, valgrind will detect an error caused by openmp, 
  which from what i've read is normal
