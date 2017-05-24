/**
 * @file
 * @author  Sven Fleischer
 * @version 1.0

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
    while (current->next != NULL) {
        GeoMean = GeoMean * pow(current->val, 1./often);
        current = current->next;
    }
    GeoMean = GeoMean * pow(current->val, 1./often);
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
    while(*(line+iter)=='0' || *(line+iter)=='1' || *(line+iter)=='2' || *(line+iter)=='3' || *(line+iter)=='4' || *(line+iter)=='5' || *(line+iter)=='6' || *(line+iter)=='7' || *(line+iter)=='8' || *(line+iter)=='9'){
			++iter;
		}
    return iter;		
}


	
int main(int argc, char *argv[]){
FILE *fp;
    char *bla;
    char delimiter[] = ";";
    char *ptr;
    int lol;
    int iter;    
    int pointer=-1; 
    size_t len = 0;
    double temp;    
    long int often[2];
    long int counter = 0;
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
	if (*(line) != '\n' && *(line) != '#'){
		iter=1;
		iter=jumpNumber(line, iter);
		iter=jumpSpace(line, iter);
		if(*(line+iter)==';'){
			++iter;
			iter=jumpSpace(line, iter);
		}
		else {
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
		continue;
		}
		iter=jumpSpace(line,iter);
		if(*(line+iter)!=';') {
		continue;
		}
		++iter;
		iter=jumpSpace(line, iter);
		if (*(line+iter)=='N'){
			continue;
		}
		iter=jumpNumber(line,iter);
		if(*(line+iter)=='.'){
			++iter;
			iter=jumpNumber(line,iter);
		}
		iter=jumpSpace(line, iter);
		if(*(line+iter)=='#'||*(line+iter)=='\n'||*(line+iter)=='e'){
			ptr = strtok(line, delimiter);
			ptr = strtok(NULL, delimiter);
			ptr = strtok(NULL, delimiter);
			if (pointer == 0){
				if (often[pointer] == -1){
					temp = strtod(ptr, &bla);
					val1->val = temp;
					val1->next = NULL;
					often[pointer]=1;  
				}		
				else {
					temp = strtod(ptr, &bla);
					val11->next = malloc(sizeof(node_t));
					val11->next->val = temp;
					val11->next->next = NULL;
					val11 = val11->next;	
					++often[pointer];
				}
			}
			else{
				if (often[pointer] == -1){
					temp = strtod(ptr, &bla);
					val2->val = temp;
					val2->next = NULL;
					often[pointer]=1;
				}		
				else {
					temp = strtod(ptr, &bla);
					val22->next = malloc(sizeof(node_t));
					val22->next->val = temp;
					val22->next->next = NULL;
					val22 = val22->next;	
					++often[pointer];
				}			
			}
		}				
    	}
    }
   printf("%s has %li lines \n",argv[1], counter);	
   printf("Valid values Loc1: %li, with with GeoMean: %f \n",often[0], getGeoMean(val1, often[0]));
   printf("Valid values Loc2: %li, with with GeoMean: %f \n",often[1], getGeoMean(val2, often[1]));
   
   free(line);
   exit(EXIT_SUCCESS);
   return argc;

}
