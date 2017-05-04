#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Beim kompilieren muss die Option 'lm' hinzugefügt werden: 'gcc ex1.c -lm -o ex1'

int main(int argc, char *args[]){
	FILE * fp;
	char * line = NULL;
	ssize_t lineLength = 0;

	int len = 0, len1 = 0, len2 = 0;
	double prod1 = 0, prod2 = 0;
	double number;
	int read;
	int hashPosition;
	char delimiter[] = "; ";
	char *ptr,*ptrClass,*ptrFloat;
	
	fp = fopen(args[1], "r");

	if (fp == NULL) exit(EXIT_FAILURE);

	while ((read = getline(&line, &lineLength, fp)) != -1) {
		len++;
		if (strncmp(line, "#",1) == 0) continue;
		line = strtok(line, "'#");

		ptr = strtok(line, delimiter);
		ptrClass = strtok(NULL, delimiter);
		ptrFloat = strtok(NULL, delimiter);

		if (ptrFloat == NULL) continue;
		number = atof(ptrFloat);

		if ( number < 0 | number != number) continue; // 'number != number' schließt NaN aus. 

		if (strcmp(ptrClass,"2") == 0){
			prod2 += log(number);
			len2++;
			continue;
		}
		if ((strcmp(ptrClass,"1") == 0) ){
			prod1 += log(number);
			len1++;
		}
	}
	printf("File: %s with %i lines\n", args[1], len);
	printf("Valid values Loc1: %i with GeoMean: %lf\n", len1, exp(prod1 / len1));
	printf("Valid values Loc2: %i with GeoMean: %lf\n", len2, exp(prod2 / len2));

	fclose(fp);
	if (line)
		free(line);
	exit(EXIT_SUCCESS);
	return 0;
}
