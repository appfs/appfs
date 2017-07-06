# EXERCISE 1
1. Download/checkout the program ex1_gen from this repository.
2. Run this program as follows:

    `./ex1_gen 100000000 > ex1.dat.`

   This generates a file ex1.dat, which should encompass around 100 million lines. You can check it with the shell command:

    `wc -l ex1.dat`

   The size of the file should be around 2GB. If you use Windows or Mac, you cannot execute the ex1_gen, but you can directly download
   the generated file from the course website: https://www.zib.de/koch/lectures/ss2017_appfs.php
   Each line of ex1.dat should consist of

   - a sequence-number,
   - a location (1 or 2),and
   - a floating point value > 0.

   Empty lines are allowed. Comment lines start	with a ”#”. Everything after a ”’#” on a line should be ignored.

3. Write a program that

   a) reads in and internally stores the data from ex1.dat

   b) computes the geometric mean for each location. Be aware that there might be some errors in the data. The output should look similar to:

```
    File: ex1.dat with 100001235 lines
    Valid values Loc1: 50004598 with GeoMean: 36.7817
    Valid values Loc2: 49994703 with GeoMean: 36.7825
```

4. How to hand in your exercise:

   a) Check in the source code into github as explained in the lecture.

   b) use this command to get the runtimes of your program:

    `time ex1 ex1.dat`

   c) Send the output of your program as in 3b) and the time measure as in 4b) with subject: APPFS ex1 firstnamesecondname per email to <koch@zib.de>


**Deadline:   April 27, 16:00**
