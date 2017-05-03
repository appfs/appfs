#define _GNU_SOURCE
#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include <math.h> 

#define MAX_LINE_LEN 512

    
typedef struct node {
    double val;
    struct node * next;
} node_t;


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

void printList(node_t * head) {
    node_t * current = head;

    while (current != NULL) {
        printf("%f\n", current->val);
        current = current->next;
    }
}


	
int main(int argc, char *argv[]){
FILE *fp;
    char *line = (char*) malloc(MAX_LINE_LEN * sizeof(char));
    size_t len = 0;
    long int often1 = 0;
    long int often2 = 0;
    double temp;
    char delimiter[] = " ;";
    char *ptr;
    long int counter = 0;
    char *bla;
    int lol;

    
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
		ptr = strtok(line, delimiter);
		ptr = strtok(NULL, delimiter);
		if (*(ptr) == '1' && strlen(ptr)==1){
			ptr = strtok(NULL, delimiter);
 			if (*ptr != 'N'){
 				if (often1 == 0){
	 				 	temp = strtod(ptr, &bla);

						val1->val = temp;
    						val1->next = NULL;
						++often1;    						
 				}	
 					
 				else{
 					temp = strtod(ptr, &bla);
    					val11->next = malloc(sizeof(node_t));
    					val11->next->val = temp;
    					val11->next->next = NULL;
 					val11 = val11->next;	
					++often1; 
				}
 			}
				
		}	
		else if (*(ptr) == '2' && strlen(ptr)==1){
			ptr = strtok(NULL, delimiter);
 			if (*ptr != 'N'){
 				if (often2 == 0){
	 				 	temp = strtod(ptr, &bla);
						val2->val = temp;
    						val2->next = NULL;	
						++often2;
	 			}	
 				else{
 					temp = strtod(ptr, &bla);
    					val22->next = malloc(sizeof(node_t));
    					val22->next->val = temp;
    					val22->next->next = NULL;
 					val22 = val22->next;	
					++often2; 
				}
 			}			
			
		}
    	}
    }

   printf("%s has %li lines \n",argv[1], counter);	
   printf("Valid values Loc1: %li, with with GeoMean: %f \n",often1, getGeoMean(val1, often1));
   printf("Valid values Loc2: %li, with with GeoMean: %f \n",often2, getGeoMean(val2, often2));
   
   free(line);
   exit(EXIT_SUCCESS);
   return argc;

}
