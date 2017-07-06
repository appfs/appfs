# Exercise 1
* compile: `g++ -std=c++11 -O3 ex1.cxx -o ex1`
* run: `./ex1 ex1.dat`
* **ex1.cxx** accepts data filename as argument. The geometric mean is 
calculated favouring arithmetic multiplication until value is close-to-overflow, 
after which log is taken. Rejected values, excluding comments and empty lines,
are printed.
    * Lines read: 100001235.
    * Time taken: ~1m.
    * Valid Values at Loc1: 50004412.
    * Valid Values at Loc2: 49994541.
    * Number of Loc1 lines with whitespace at start or end: 54
    * Number of Loc2 lines with whitespace at start or end: 40
 
