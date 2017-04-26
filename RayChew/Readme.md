# Exercise 1
Ray Chew (380421)
1. **ex1.cxx** is a "faster" method using only getline() with poorer handling of 
exceptions. The geometric mean is calculated favouring arithmetic multiplication 
until value is close-to-overflow, after which log is taken.
    * Time taken: ~28s.
    * Valid Values at Loc1: 50004747.
    * Valid Values at Loc2: 49994854.
    * Lines skipped: 1631.
2. **ex1_stringstream.cxx** is a "slower" method using istringstream() with 
better handling of exceptions. The geometric mean is calculated with the same 
method as in ex1.cxx.
    * Time taken: ~1m 25s.
    * Valid Values at Loc1: 50005278.
    * Valid Values at Loc2: 49995372.
    * Lines skipped: 583.
3. **ex1_logall.cxx**, the geometric mean is calculated favouring the log() 
function, i.e. all values are logged and summed. To test the feasibility of 
using the log() function. Time taken: ~1m 38s.
4. **ex1_nolog.cxx**, the geometric mean is calculated favouring arithmetic 
methods, avoiding the use of the log() function entirely. Time taken: ~1m 28s.