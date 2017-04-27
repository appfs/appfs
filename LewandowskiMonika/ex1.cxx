//Exercise 1, Monika Lewandowski (379960)
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
using namespace std;
 
int main()
{   
    int l=0;
    int seqNo;
    int locNo;
    double val;

    double prodValues1 = 1.0;
    double prodValues2 = 1.0; 
    int totalValues1 = 0;
    int totalValues2 = 0;
    double logval1=0, logval2=0;
    double valMax = 1e64; // maximal value to avoid overflow
    
    string input, line, entry1, entry2, entry3;

    cout << "Please enter the name of your file:\n>";
    getline(cin, input);

    ifstream valuesFile(input); //input file stream

    while (getline(valuesFile, line))
    {
        istringstream iss(line); //input string stream 
        
        iss >> entry1 >> entry2 >> entry3; //get the elements of the columns of each line
        if (entry1 == "#" || entry1 == " ")
        {
          getline(valuesFile, line); //skip lines with '#' or empty lines
         }
        else
        {
        val = stof(entry3); //convert string to float
        if ((entry2.size() == 2) && (isnan(val) == false)) //avoid GeoMean = nan and error for invalid_argument   
        {
        locNo = stoi(entry2); //convert string to int
         }
        else
        { 
            locNo = 0; //avoid GeoMean = nan
        }
        if (locNo == 1)//calculate GeoMean for location 1
        {   
            prodValues1 *= val;
            totalValues1 += 1;
            if (prodValues1 > valMax)//avoid overflow and use log
            {
               logval1 += log(prodValues1);
               prodValues1 = 1.0;
             }
        }
        else if (locNo == 2)//calculate GeoMean for location 2
        {
           prodValues2 *= val;
           totalValues2 += 1;
           if (prodValues2 > valMax)
           {
              logval2+=log(prodValues2);
              prodValues2 = 1.0;
            }
         } 
    } l += 1;
    }
   double geoMean1 = exp((logval1 + log(prodValues1))/totalValues1); //calculate the GeoMean as combination of log and exp
   double geoMean2 = exp((logval2 + log(prodValues2))/totalValues2);

   cout << "File: " << input << " with " << l << " lines" << endl;
   cout << "Valid values Loc1: " << totalValues1 << " with GeoMean: " << geoMean1 <<endl;
   cout << "Valid values Loc2: " << totalValues2 << " with GeoMean: " << geoMean2 <<endl;
   
return 0;
}


