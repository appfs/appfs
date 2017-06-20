ex6.cpp is a solution to exercise 5 with two additional features:
  - it prints the wall clock and user time after completion
  - it provides two command line options (--m1 and --m2) to choose whether my own implementation (m1) of dijkstra
    or the one from the boost package (m2) should be used. One of the two has to be chosen.
   
The Makefile provides commands for compilation, documentation, coverage and check. For coverage, the file world666.gph is used which needs to be in the same directory

Please note that you need boost to compile and that two non-header-only packages from boost are used (timer and program_options)



The output should look like this:


  [information about which algorithm is used]

  RESULT VERTEX 42

  RESULT DIST 5553

  [information about running time]
