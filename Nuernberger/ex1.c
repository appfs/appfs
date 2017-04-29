
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

FILE* dat;
int next;

void nextLine()
{
	while (next != '\n' && next != EOF) 
		next = fgetc(dat);
}

void main()
{
	int totalLines = 0, c1 = 0, c2 = 0;
	double v1 = 0, v2 = 0;

	dat = fopen("ex1.dat", "r");

	if (dat == NULL) 
	{
		perror("Error opening ex1.dat"); 
		return;
	}

	for (next = fgetc(dat); next != EOF; next = fgetc(dat))
	{
		totalLines++;

		if(next == '#')
		{
			nextLine();
			continue;
		}

		short int badLine = 0;
		while (next != ';' && next != EOF)
		{
			if (next < 48 || 57 < next)
			{
				//printf("Error code 1\n");
				badLine = 1;
				break;
			}

			next = fgetc(dat);

			if (next == 35)
			{
				badLine = 1;
				break;
			}	
		}

		if (badLine)
		{
			nextLine();
			continue;
		}

		next = fgetc(dat);

		while (next == 32)
		{
			next = fgetc(dat);
		}

		int* c;
		double* v;

		if (next == 49)
		{
			c = &c1;
			v = &v1;
		}
		else if (next == 50)
		{
			c = &c2;
			v = &v2;
		}
		else
		{
			//printf("Error code 2\n");
			nextLine();
			continue;
		}
	
		next = fgetc(dat);

		while (next == 32)
		{
			next = fgetc(dat);
		}
		if (next == ';')
		{
			next = fgetc(dat);
			while (next == ' ')
			{
				next = fgetc(dat);
			}
		}
		else
		{
			//printf("Error code 3\n");
			nextLine();
			continue;
		}

		double num = 0;
		
		if (47 < next && next < 58)
		{
			num += next - 48;
			next = fgetc(dat);

			while (47 < next && next < 58)
			{
				num *= 10;
				num += next - 48;
				next = fgetc(dat);
			}
		}
		else if (next != 46)
		{
			//printf("Error code 4\n");
			nextLine();
			continue;
		}

		if (next == 46)
		{
			next = fgetc(dat);
			for(int d = 10; 47 < next && next < 58; next = fgetc(dat), d *= 10)
			{
				num += ((double) next - 48)/d;
			}
		}

		if (next != 10)
		{
			//printf("Error code 5\n");
			nextLine();
			continue;
		}

		*c += 1;
		*v += log(num);
		
	}
	v1 = exp(v1/c1);
	v2 = exp(v2/c2);
	printf("File: ex1.dat with %d lines\nValid values Loc1: %d with GeoMean: %f \nValid values Loc2: %d with Geomean: %f \n", totalLines, c1, v1, c2, v2);
}
