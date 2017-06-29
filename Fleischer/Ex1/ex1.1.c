/**
 * @file
 * @author  Sven Fleischer
 * @version 1.1

 *
 * @section DESCRIPTION
 *
 * This programm computes the Geometric Mean of loc1 and loc2 of a given file. Uses now an array instead of a list.
 */


#define _GNU_SOURCE
#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include <math.h> 
#include <assert.h>
#include <ctype.h>
#include <fenv.h>

#define MAX_LINE_LEN 512





/**
* Computes the Geometric Mean of list
*
* @param array with values
* @param amount of values
* @return The Geometric Mean of an array
*/


double getGeoMean(double* val, long int often){
	double GeoMean = 1;
	assert(often !=0);
	for(size_t i = 0; i < often; ++i){
		GeoMean = GeoMean * pow(val[i], 1./often);    
        	assert(!fetestexcept(FE_INVALID | FE_OVERFLOW | FE_UNDERFLOW));
	}
	return GeoMean;
}

/**
* Jumpes to the next non space element of a char.
*
* @param pointer to a char
* @param index element of a string
* @return index of the first non space element of a string starting from a given index
*/

int jumpSpace(char* line, int iter){
    while(*(line+iter) == ' '){
    ++iter;
    }
    return iter;
}

/**
* Jumpes to the next non digit element of a char.
*
* @param pointer to a char
* @param index element of a string
* @return index of the first non digit element of a string starting from a given index
*/

int jumpNumber(char *line, int iter){
    while(*(line+iter) > 47 && *(line+iter) < 58){
			++iter;
		}
    return iter;		
}

/**
* Computes the Geometric Mean of loc1 and loc2 of a file
*
* @param life at the call of the programm
* @return The Geometric Mean of loc1 and loc2 with the number of valid loc1 and loc2 lines and the amont of lines in the file
*/
	
int main(int argc, char *argv[]){
FILE *fp;
    char delimiter[] = ";";
    char *ptr;
    char *bla;
    int lol;
    int iter;    
    int pointer=-1; 
    size_t len = 0;
    double temp;    
    long int often[2];
    long int counter = 0;
    long int linenr = 1;
    char *line = (char*) malloc(MAX_LINE_LEN * sizeof(char));
    often[0] = -1;
    often[1] = -1;
    double* val1 = (double*) malloc(10*sizeof(double));
    double* val2 = (double*) malloc(10*sizeof(double));
    long int size1 = 10;
    long int size2 = 10;    



   fp = fopen(argv[1], "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

   while ((lol=getline(&line, &len, fp)) != -1) {
	++counter;
	assert(line != NULL);
	if (*(line) != '\n' && *(line) != '#'){
		iter=1;
		iter=jumpNumber(line, iter);
		iter=jumpSpace(line, iter);
		if(*(line+iter)==';'){
			++iter;
			iter=jumpSpace(line, iter);
		}
		else {
			++linenr;
			continue;
		}
		if(*(line+iter)=='1'){
			pointer = 0;
			++iter;
		}
		else if(*(line+iter)=='2'){
			pointer = 1;
			++iter;
		}
		else {
			++linenr;
			continue;
		}
		iter=jumpSpace(line,iter);
		if(*(line+iter)!=';') {
			++linenr;
			continue;
		}
		++iter;
		iter=jumpSpace(line, iter);
		if (*(line+iter)=='N'){
			++linenr;
			continue;
		}
		iter=jumpNumber(line,iter);
		if(*(line+iter)=='.'){
			++iter;
			iter=jumpNumber(line,iter);
		}
		iter=jumpSpace(line, iter);
		if(*(line+iter)=='#'||*(line+iter)=='\n'||*(line+iter)=='e'){

			if (atol(line) != linenr){
				++linenr;
        			continue;
			}
			ptr = strtok(line, delimiter);
			++linenr;
			ptr = strtok(NULL, delimiter);
			ptr = strtok(NULL, delimiter);
			temp = strtod(ptr, &bla);
			if (!isnormal(temp)){
				printf("Could not convert value in line %s and the string. value is:%s \n", line, ptr);
				continue;
			}
			if (pointer == 0){
				if (often[pointer] == -1){
					val1[0]= temp;
					often[pointer]=1;  
				}		
				else {
					if (often[pointer]< size1){
						val1[often[pointer]]= temp;	
						++often[pointer];
					}
					else{
						val1 = realloc(val1, sizeof(double)*often[pointer]*250);
						
						size1 = often[pointer]*250;

						val1[often[pointer]]= temp;	
						++often[pointer];									
					}
				}
			}
			else{
				if (often[pointer] == -1){
					val2[0]= temp;
					often[pointer]=1;  
				}		
				else {
					if (often[pointer] < size2){
						val2[often[pointer]]= temp;
						++often[pointer];
					}
					else{
						val2 = realloc(val2, sizeof(double)*often[pointer]*250);
						val2[often[pointer]]= temp;
						size2 = often[pointer]*250;				
						++often[pointer];									
					}
				}			
			}
		}
		else{
			++linenr;
			ptr = strtok(line, delimiter);
			ptr = strtok(NULL, delimiter);
			ptr = strtok(NULL, delimiter);
			ptr = strtok(NULL, delimiter);
			if (ptr != NULL){
				ptr = strtok(NULL, delimiter);
					if (ptr !=NULL) ++linenr;
				
			}
		}	
    	}
    }
   printf("%s has %li lines \n",argv[1], counter);
   if (often[0]!=-1){	
   	printf("Valid values Loc1: %li, with with GeoMean: %f\n",often[0], getGeoMean(val1, often[0]));
   }
   else{
   	printf("Valid values Loc1: 0\n");
   }
   if (often[1]!=-1){
   	printf("Valid values Loc2: %li, with with GeoMean: %f \n",often[1], getGeoMean(val2, often[1]));
   }
   else{
   	printf("Valid values Loc2: 0\n");
   }
   free(line);
   free(val1);
   free(val2);
   exit(EXIT_SUCCESS);
   return argc;

}
