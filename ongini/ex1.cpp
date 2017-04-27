#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <math.h>

int main()
{
  std::string line;
  int lnum,l,l2=0;
  int l1=0;
  double n,n1,n2=0.0;
  std::ifstream file;
  while(getline (std::cin,line))
    {
      lnum= lnum+1;
      std::stringstream ss(line);
      if (line[0]=='#'|line[0]==' '){;
      }else
	{
      	    getline (ss,line,' ');
      	    getline (ss,line,' ');
       	    l = std::atoi (line.c_str());
	    getline (ss,line,' ');
	    n= std::atof (line.c_str());
	    if (isnan(n)|n==0){continue;};
      	    switch(l)
	      {
      		case 1:{l1 = l1 +1 ; n1 = n1+log(n); break;}
      		case 2:{l2 = l2 +1 ; n2 = n2+log(n); break;}
      	      }
	}
  }
  std::cout << "File ex1.dat with " << lnum << " lines" << std::endl;
  std::cout << "Valid values Loc1:" << l1 << " with GeoMean:" <<exp(n1/l1) <<std::endl; 
  std::cout << "Valid values Loc2:" << l2 << " with GeoMean:" <<exp(n2/l2) << std::endl;
  return(0);
}
