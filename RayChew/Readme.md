# Exercise 1
Ray Chew (380421)
* **ex1.cxx** accepts data filename as argument. The geometric mean is 
calculated favouring arithmetic multiplication until value is close-to-overflow, 
after which log is taken. Does not handle cases with space(s) in SeqNo.
    * Time taken: ~1m 45s.
    * Valid Values at Loc1: 50004653.
    * Valid Values at Loc2: 49994758.
    * Lines skipped: 1824.