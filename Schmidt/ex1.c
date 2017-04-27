#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double dlog100;

// computes the logarithm with base 100
double log100(double x)
{
	if(x > 0)
		return log(x)/dlog100;
	else
		return 0;
}

int main(int argc, char **argv)
{
	dlog100 = log(100);
	// set the file name or a standard file if there is no argument
	char *filename = "ex1.dat";
	if(argc >= 2)
		filename = argv[1];
	
	// try to open the file
	FILE *f;
	f = fopen(filename, "r");
	
	// exit if the given file cannot be opened
	if(f == NULL)
	{
		fprintf(stdout, "The file could not be opened, program exits.");
		return 1;
	}
	
	char line[100];
	int loc;
	double val;
	
	long double logsum1 = 0, logsum2 = 0;
	
	int cnt1 = 0;
	int cnt2 = 0;
	int overallcnt = 0;
	
	// start reading the data (first string contains sequence-number and is ignored)
	while(fscanf(f, "%s", line) != -1)
	{
		// ignore the line if it starts with a "#"
		if(line[0] == '#')
			fgets(line, sizeof line, f);
		else
		{
			// get the remaining data consisting of the location and value
			// 
			fscanf(f, "%d; %lf", &loc, &val);
			if(loc == 1)
			{
				logsum1 += log100(val);
				++cnt1;
			}
			else if(loc == 2)
			{
				logsum2 += log100(val);
				++cnt2;
			}
			fgets(line, sizeof line, f);
		}
		++overallcnt;
	}
	
	fclose(f);
	// output the necessary data
	fprintf(stdout, "File: %s with %d lines\n", filename, overallcnt);
	fprintf(stdout, "Valid values Loc1: %d with GeoMean: %lf\n", cnt1, pow(100, logsum1/cnt1));
	fprintf(stdout, "Valid values Loc2: %d with GeoMean: %lf\n", cnt2, pow(100, logsum2/cnt2));
		
	return 0;
}

