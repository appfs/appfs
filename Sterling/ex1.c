#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

// Beim kompilieren muss die Option 'lm' hinzugefügt werden: 'gcc ex1.c -lm -o ex1'
// Mit der Option '-v' hinter dem Dateinamen, werden alle "Fehler" ausgegeben: 'time ./ex1 ../ex1.dat -v'

/* Output of my Linux running as VM:
	File: ../ex1.dat with 100001235 lines
	Valid values Loc1: 50004777 with GeoMean: 36.781715
	Valid values Loc2: 49994865 with GeoMean: 36.782540

	real	1m14.107s
	user	1m11.204s
	sys	0m2.832s
*/

#define EXT_SIZE	(1024*1024)
#define MAX_LINE_LEN	512
#define LOCS		2
#define DELIMITER	";"

void add_float(int loc,double fl);
void print_error_and_continue(char* line, char* error);
void validate_line(char* line);
double geom_mean_pow(const double* x, const size_t count);
// idea of realloc array was taken from TKoch. PS: using the variables as global variables is my lazy way of 
size_t  loc_next  [LOCS];  // the index of the next number to store
size_t  loc_size  [LOCS];  // the size of the arrays to store the numbers
double* loc_value [LOCS];  // array to store the numbers
bool	VERBOSE = false;
int 	len	= 0;

int main(int argc, char *args[]){
	if (argc < 1) return 0;
	int j = argc;
	while(j > 0 && !VERBOSE){
		if(strcmp(args[j-1], "-v") == 0){
			VERBOSE = true;
		}
		j--;
	}
	for(int i = 0; i < LOCS; i++){
		loc_next [i] = 0;         // we start empty 
		loc_size [i] = EXT_SIZE;  // initial size
		loc_value[i] = malloc(loc_size[i] * sizeof(loc_value[i][0]));
	}
	FILE * fp;
	char * line = NULL;
	ssize_t lineLength = 0;
	int read;

	fp = fopen(args[1], "r");
	if (fp == NULL) exit(EXIT_FAILURE);


	while ((read = getline(&line, &lineLength, fp)) != -1) {
		len++;
		validate_line(line);
	}
	printf("File: %s with %i lines\n", args[1], len);
	for(int i = 0; i < LOCS; i++){
		printf("Valid values Loc%i: %li with GeoMean: %lf\n", i + 1, loc_next[i], geom_mean_pow(loc_value[i],loc_next[i]));
	}

	fclose(fp);
	if (line)
		free(line);
	exit(EXIT_SUCCESS);
	return 0;
}

double geom_mean_pow(const double* x, const size_t count){
	double res 	= 1.0;
	double factor 	= 1.0/count;
	for (int i = 0; i < count; i++){
		res *= pow(x[i], factor);
	}
	return res;
}
void add_float(int loc,double fl){
	if (loc_next[loc-1] == loc_size[loc-1]){
		loc_size [loc-1] += EXT_SIZE;
		loc_value[loc-1] = realloc(loc_value[loc-1], loc_size[loc-1] * sizeof(loc_value[loc-1][0]));
	}
	loc_value[loc-1][loc_next[loc-1]] = fl;
	loc_next[loc-1]++;
}
void print_error(char* line, char* error){ 
	if (VERBOSE == 0) return;
	printf("Error [%10s,%9i]: %s", error, len, line); 
}
void validate_line(char* line){
	// empty line	
	if (strcmp(line,"\n") == 0){
		print_error(line,"empty"); return;
	}
	char lineTMP[MAX_LINE_LEN];
	strcpy(lineTMP, line);
	lineTMP[MAX_LINE_LEN] = '\0';

	// line starting with # is neglected
	if (strncmp(lineTMP, "#",1) == 0){
		print_error(line,"comment"); return;
	}
	strtok(lineTMP, "'#");
	// splitting line
	char*	ptr		= strtok(lineTMP, DELIMITER);
	char*	ptrClass	= strtok(NULL, DELIMITER);
	int	loc		= atoi(ptrClass);
	// False or none location
	if (loc > LOCS || loc < 1){
		print_error(line,"falseLOC"); return;
	}

	char* ptrFloat = strtok(NULL, DELIMITER);
	
	// no float as third argument or less than 3 arguments
	if (ptrFloat == NULL){
		print_error(line,"noFloat"); return;
	}
	double 	number 	= atof(ptrFloat);
	// non-positive or NAN value
	if (number <= 0.0 || !isnormal(number)){
		print_error(line,"falseValue"); return;
	}
	add_float(loc,number);
}
