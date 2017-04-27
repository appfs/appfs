#include "program.h"

#define BUFFER_SIZE (1 * 1024 * 1024)

char * read_data()
{
	int x = 0, data_length = 0;
	long count = 1;

	char * data = NULL;

	unsigned char buffer[BUFFER_SIZE]; // 1 MiB buffer
	FILE * fp = NULL;
	fp = fopen( "ex1-fake.dat", "r" );

	data = calloc(1, sizeof(char));

	if(fp)
	{
		for(;;)
		{
			printf("\nTotal <= %d MiB\n", count);
			count++;

			fread(buffer, BUFFER_SIZE-1, 1, fp);
			memcpy(buffer+BUFFER_SIZE, "\0", 1);

			data_length += BUFFER_SIZE;
			data = realloc(data, (data_length + 1) * sizeof(char));
			strcat(data, buffer);

			if(feof(fp)) 
			{
				break;
			}
			else if(ferror(fp))
			{
				fprintf(stderr, "\nError: Could not read text file.\n");
				break;
			}
		}
	}else{
		fprintf(stderr, "\nError: Could not open text file.\n");
	}
	fclose(fp);
	return data;
}
