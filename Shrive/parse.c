#include "ex1.h"

struct double_xy geo_mean_rows(struct row *conductor, int count1, int count2)
{
	int count;
	double result;
	struct double_xy geo_mean;
	geo_mean.x = 1.;
	geo_mean.y = 1.;

	if(1 == conductor->location) geo_mean.x *= pow(conductor->value, 1./count1);
	if(2 == conductor->location) geo_mean.y *= pow(conductor->value, 1./count2);
	while( conductor->next_row != NULL)
	{
		conductor = conductor->next_row;
		if(1 == conductor->location) geo_mean.x *= pow(conductor->value, 1./count1);
		if(2 == conductor->location) geo_mean.y *= pow(conductor->value, 1./count2);
	}
	return geo_mean;
}

struct int_xy get_lines(struct row * conductor, char * file_name)
{		
	int i = 0, seq_no, location, count1 = 0, count2 = 0;
	double value;
	char line[MAX_LINE_LENGTH];
	FILE * file = fopen(file_name, "r");
	struct int_xy count;

	if (file)
	{
		while (fgets(line, sizeof(line), file))
		{
			if(strncmp(line, "#", 1) != 0)
			{			
				if( 3 == sscanf( line, "%d; %d; %lf", &seq_no, &location, &value ) && DBL_EPSILON < value)
				{
					if(i == 1)
					{
						conductor->next_row = calloc( 1, sizeof(struct row));
						conductor = conductor->next_row;
						conductor->next_row = NULL; 	
					}
					conductor->seq_no = seq_no;
					conductor->location = location;
					conductor->value = value;
				
					if( 1 == location ) count1++;
					if( 2 == location ) count2++;
					i = 1;
				}
			}
		}	

	}
	if(ferror(file)) fprintf(stderr, "Error reading file");

	fclose(file);
	count.x = count1;
	count.y = count2;
	return count;
}

