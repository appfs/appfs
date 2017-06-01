/**
 * @file
 * @author  Sven Fleischer
 * @version 1.1

 *
 * @section DESCRIPTION
 *
 * This programm computes the Geometric Mean of loc1 and loc2 of a given file.
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
* A simple list
*/ 
typedef struct node {
    double val;
    struct node * next;
} node_t;


/**
* Computes the Geometric Mean of list
*
* @param head element of a list
* @param size of the list
* @return The Geometric Mean of a list
*/

double getGeoMean(node_t * head, long int often){
    node_t * current = head;
    double GeoMean = 1;
    assert(often == 0);
    while (current->next != NULL) {
	assert((current->val) > 0);	    
        GeoMean = GeoMean * pow(current->val, 1./often);    
        assert(!fetestexcept(FE_INVALID | FE_OVERFLOW | FE_UNDERFLOW));
        current = current->next;
    }
    assert((current->val) > 0);	
    GeoMean = GeoMean * pow(current->val, 1./often);
    assert(!fetestexcept(FE_INVALID | FE_OVERFLOW | FE_UNDERFLOW));
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
    
    node_t * val1 = NULL;
    val1 = malloc(sizeof(node_t));
    node_t *  val11 = val1;
    
    node_t * val2 = NULL;
    val2 = malloc(sizeof(node_t));
    node_t * val22 = val2;


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
				printf("Could not convert value in line %s and the string. Value is:%s \n", line, ptr);
				continue;
			}
			if (temp<= 0){
				printf("Converted value makes no sense for Geometric Mean. Value is:%s \n", line, ptr);
				continue;
			}
			if (pointer == 0){
				if (often[pointer] == -1){
					val1->val = temp;
					val1->next = NULL;
					often[pointer]=1;  
				}		
				else {
					val11->next = malloc(sizeof(node_t));
					val11->next->val = temp;
					val11->next->next = NULL;
					val11 = val11->next;	
					++often[pointer];
				}
			}
			else{
				if (often[pointer] == -1){
					val2->val = temp;
					val2->next = NULL;
					often[pointer]=1;
				}		
				else {
					val22->next = malloc(sizeof(node_t));
					val22->next->val = temp;
					val22->next->next = NULL;
					val22 = val22->next;	
					++often[pointer];
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
   exit(EXIT_SUCCESS);
   return argc;

}
