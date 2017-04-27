#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include <math.h> 
#define MAX_LINE_LEN 512


void main(int argc[], char *argv[]){
FILE *fp;
    char *line = malloc(sizeof(char*));
    size_t len = 0;
    long double loc1 = 1;
    long double loc2 = 1;
    long int often1 = 0;
    long int often2 = 0;
    long double temp;
    char delimiter[] = " ;";
    char *ptr;
    long int counter = 0;
    char *bla;
    

   fp = fopen(argv[1], "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

   while ((getline(&line, &len, fp)) != -1) {
	counter++;
	
	if (*(line) != '\n' &&*(line) != '#'){
		ptr = strtok(line, delimiter);
		ptr = strtok(NULL, delimiter);
		if (*(ptr) == '1' && strlen(ptr)==1){
			ptr = strtok(NULL, delimiter);
 			if (*ptr != 'N'){
 				if (often1 == 0){
	 				 	temp = strtod(ptr, &bla);
						loc1 = loc1*temp;
						often1++;
 				}	
 					
 				else{
 					temp = strtod(ptr, &bla);
					temp = pow(temp, 1./often1);
					loc1 = loc1*temp;
					loc1 = pow(loc1, (1.*often1/(often1+1)));
					often1++; 
				}
 			}
				
		}	
		else if (*(ptr) == '2' && strlen(ptr)==1){
			ptr = strtok(NULL, delimiter);
 			if (*ptr != 'N'){
 				if (often2 == 0){
	 				 	temp = strtod(ptr, &bla);
						loc2 = loc2*temp;
						often2++;
	 			}	
 				else{
 					temp = strtod(ptr, &bla);
					temp = pow(temp, 1./often2);
					loc2 = loc2*temp;
					loc2 = pow(loc2, (1.*often2/(often2+1)));
					often2++; 
				}
 			}			
			
		}
    	}
    }
   printf("%s has %li lines \n",argv[1], counter);	
   printf("Valid values Loc1: %li, with with GeoMean: %Lf \n",often1, loc1);
   printf("Valid values Loc2: %li, with with GeoMean: %Lf \n",often2, loc2);
   
   free(line);
    exit(EXIT_SUCCESS);

}
