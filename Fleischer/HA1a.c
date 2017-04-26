#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include <math.h> 

void main(int argc, char *argv[]){
FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    long double loc1 = 1;
    long double loc2 = 1;
    long long often1 = 0;
    long long often2 = 0;
    char str;
    long double temp;
    char delimiter[] = " ;";
    char *ptr;
    long long counter = 0;
    char *bla;
    

   fp = fopen(argv[1], "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

   while ((getline(&line, &len, fp)) != -1) {
       // printf("Retrieved line of length %zu :\n", read);
    	//printf("%s \n", line);
    			   
        counter++;
        //printf("HA\n");
	if (*(line) != '\n' &&*(line) != '#'){
		ptr = strtok(line, delimiter);
		//if (ptr == " "){
		//	ptr = strtok(line, delimiter);}
		ptr = strtok(NULL, delimiter);
		if (*(ptr) == '1' && strlen(ptr)==1){
			ptr = strtok(NULL, delimiter);
 			if (*ptr != 'N'){
 				if (often1 == 0){
	 				 						temp = strtod(ptr, &bla);
	 				//if (temp != INFINITY){
						loc1 = loc1*temp;
						//if(loc2 == INFINITY) break;
						often1++;
 						//}
 					}	
 					
 				else{
 					temp = strtod(ptr, &bla);
	 				//if (temp != INFINITY){
					temp = pow(temp, 1./often1);
					loc1 = loc1*temp;
					//if(loc1 == INFINITY) break;
					loc1 = pow(loc1, (1.*often1/(often1+1)));
					often1++; 
					//}
					}
 				}
				
			}	
		else if (*(ptr) == '2' && strlen(ptr)==1){
			ptr = strtok(NULL, delimiter);
 			if (*ptr != 'N'){
 				if (often2 == 0){
	 				 						temp = strtod(ptr, &bla);
	 				//if (temp != INFINITY){
						loc2 = loc2*temp;
						//if(loc2 == INFINITY) break;
						often2++;
	 					//}
	 				}	
 				else{
 					temp = strtod(ptr, &bla);
	 				//if (temp != INFINITY){
					temp = pow(temp, 1./often2);
					loc2 = loc2*temp;
					//if(loc2 == INFINITY) break;
					loc2 = pow(loc2, (1.*often2/(often2+1)));
					often2++; 
					//}
					}
 				}			
			
		}
    	}
    	}
 //  printf("loc1: %Lf and loc2: %Lf \n" ,loc1, loc2);
   printf("%s has %li lines \n",argv[1], counter);	
   printf("Valid values Loc1: %li, with with GeoMean: %Lf \n",often1, loc1);
   printf("Valid values Loc2: %li, with with GeoMean: %Lf \n",often2, loc2);
   
   free(line);
    exit(EXIT_SUCCESS);

}
