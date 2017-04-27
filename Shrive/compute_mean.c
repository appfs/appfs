#include "program.h"

#define EPSILON 0.0001

double nth_pow(double base, double n)
{
	int i;
	long double x = 1.;
	for(i = 0; i < n; i++)
	{
		x *= base;
	}
	return x;
}

double nth_root(double base, double n)
{
	double x = 1., delta_x;
	do
	{
		delta_x = ( base / nth_pow( x, n-1 ) - x ) / n;
		x += delta_x;
	}
	while(delta_x >= EPSILON * 10 || delta_x <= -EPSILON * 10);
	return x;
}

struct xy count_data(char * input)
{
	int i = 0, seq_no, location;
	double value, count1 = 0., count2 = 0.;
	struct xy count;

	for(;;)
	{
		if(strncmp(input+i, "\0", 1) == 0) 
		{
			printf("\nCount of location 1 = %f\n", count.x);
			printf("\nCount of location 2 = %f\n", count.y);
			break;
		}	
		if(strncmp(input, "#", 1) == 0)
		{
			for(i = 0;;i++)
			{
				if(strncmp(input+i, "\0", 1) == 0) break;
				else if(strncmp(input+i, "\n", 1) == 0)
				{
					input = input+i+1;
					break;
				}
			}
		}
		else
		{
			sscanf( input, "%d; %d; %lf\n", &seq_no, &location, &value );
			if(location == 1) count.x++;

			if(location == 2) count.y++;

			for(i = 0;;i++)
			{
				if(strncmp(input+i, "\0", 1) == 0) break;
				else if(strncmp(input+i, "\n", 1) == 0)
				{
					input = input+i+1;
					break;
				}
			}
		}
	}
	return count;
}


int geo_mean_data(char * input, struct xy count)
{
	int i = 0, seq_no, location, end = 0;
	double value, geometric_mean1 = 1. , geometric_mean2 = 1.;

	for(;;)
	{
		if(strncmp(input+i, "\0", 1) == 0) 
		{
			printf("\nGeometric mean of location 1 = %f\n", geometric_mean1);
			printf("\nGeometric mean of location 2 = %f\n", geometric_mean2);
			break;
		}			
		if(strncmp(input, "#", 1) == 0)
		{
			for(i = 0;;i++)
			{
				if(strncmp(input+i, "\0", 1) == 0) break;
				else if(strncmp(input+i, "\n", 1) == 0)
				{
					input = input+i+1;
					break;
				}
			}
		}
		else
		{
			sscanf( input, "%d; %d; %lf\n", &seq_no, &location, &value );
			if(location == 1) geometric_mean1 *= nth_root(value, count.x);

			if(location == 2) geometric_mean2 *= nth_root(value, count.y);

			for(i = 0;;i++)
			{
				if(strncmp(input+i, "\0", 1) == 0) break;
				else if(strncmp(input+i, "\n", 1) == 0)
				{
					input = input+i+1;
					break;
				}
			}
		}
	}
	return 0;
}
