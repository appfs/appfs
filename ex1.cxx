//Ex1 Amey Vasulkar

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
  using namespace std;

int main()
{    

  int seqno, locno; // Declaration and Initialization of required variables
  locno=0;
  double value;
  double logval1, logval2; 
  logval1=logval2=1.0;
  double sum1, sum2;
  sum1=sum2=0;
  int n1,n2=0;
  double geomean1, geomean2;
  int l=0;
  string line, part1, part2, part3, name;
  
  ifstream myfile ("ex1-100.dat"); //Opening file to run
  name = "ex1.dat";
  
  if (myfile.is_open())
  {
	while (getline (myfile,line)) // Loop to check each line of opened file.
	{ 
		l=l+1;
		istringstream lss (line);
		lss >> part1 >> part2 >> part3 ; //To get all the entries in a line into different strings.
		
		if (part1 == "#" || part1 == "")// To omit the lines with # and blanks.
		;
		else 
		{
			value= stof(part3);
			if (part2.size()== 2 && isnan(value)== false)
				locno = stoi(part2);
			else 
				locno=0;
				
			if (locno == 1) // Computing of Geometric mean.
			{	
				n1=n1+1;
				logval1= log (value);
				sum1=sum1+logval1;
			}
			else if (locno == 2)
			{	
				n2=n2+1;
				logval2= log (value);
				sum2=sum2+logval2;
			}
						
		}
		
	}
		
	geomean1=exp(sum1/n1);
	geomean2=exp(sum2/n2);
			
	cout << "File: "<< name << " with " << l << " lines" << endl;
	cout << "Valid values Loc1: "<< n1 << " with Geomean: " << geomean1 << endl;
	cout << "Valid values Loc2: " << n2 << " with Geomean: " << geomean2 <<endl;
	
	myfile.close();
	}
		
		else 
	  
	cout << "Unable to open file"; 
  
    return 0;
}