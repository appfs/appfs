/**
 * @file
 * @author  Sven Fleischer
 * @version 1.0

 *
 * @section DESCRIPTION
 *
 * This programm verfies at first the through the shell at the moment of the call given xml file with a given shema (this file is given at second file after the xml file at the call). It writes to stdout.csv all values in the form YYYY-MM-DD; HH; amountOfPower. In stderr comes every error and every faulty value, which is having for example a non digit written in it, or a non existing time/date.
-Value
 */

#include <libxml/parser.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <libxml/xmlschemas.h>
//parse with: gcc -std=c11 -O3 xml2-config --cflags --libs`ex2.c -o ex2 -lm
//run with ./ex2 XML-File Schema
 
int main(int argc, char *argv[]){
 FILE *fptr;
 FILE *fptr2;
 FILE *fptr3;
  //FILE *fptr; 
  
  fptr = fopen("stdout.csv", "w");
  fptr2 = fopen("stderr.csv", "w");
  if (argc ==1){
  	fprintf(fptr2, "No file given");
  	exit(EXIT_FAILURE);
  }
  xmlDoc* doc;
  xmlNode* node;
  xmlNode* node1;
  xmlNode* node2;
  xmlNode* node3;
  xmlChar* date;
  xmlChar* hour;    
  xmlChar* value;
  char* printout[2]; 
  printout[0] = "";
  printout[1] = "";
  char* line = malloc(11);
  char* ptr;
  char delimiter[] = "-";
  int decider;
  int temp;
  int iter;
  char* bla; 

  doc = xmlReadFile(argv[1], NULL, 0);
 
  if (doc == NULL) {
    	fprintf(fptr2,"Could not parse the XML file %s.", argv[1]);
    	printf("Could not parse the XML file.");
    	exit(EXIT_FAILURE);
  }
  
  if((temp=xmlSchemaValidateDoc(argv[2], argv[1]))!=0 && argv[2]!=NULL){
  	fprintf(fptr2,"Validation failed");
  	if(temp<0){ 
  		fprintf(fptr2," with internal API error with error code %i.", temp);
  	}
  	else {
  		fprintf(fptr2," with %s in wrong form with error code %i.", argv[1], temp);
  	}
  	printf("Validation failed\n");
  	exit(EXIT_FAILURE);
  }
  
  node = xmlDocGetRootElement(doc);
 
  node = node->children;
  while(node!=NULL){
	  if(node->type == XML_ELEMENT_NODE){

	  	date = xmlGetProp(node, "date");
	  	//printf("date; %s\n", date);
	  	node1 = node->children;

	  	while(node1!=NULL){
	  		if(node1->type == XML_ELEMENT_NODE){
		  		node2 = node1->children;
		  		while(node2!=NULL){
		  			while(node2->type == XML_ELEMENT_NODE){
			  			node2 = node2->next; if (node2==NULL) break;
			  		}
			  		while(!strcmp(node2->name,"text")|| !strcmp(node2->name,"comment")){
			  			node2 = node2->next;if (node2==NULL) break;
			  		}
			  		if(node2==NULL) break;
			  		hour = xmlGetProp(node2, "hour");
			  		node3 = node2->children;
			  		while(node3->type == XML_ELEMENT_NODE){
			  			node3 = node3->next;
			  		}
			  		while(!strcmp(node3->name,"text")|| !strcmp(node3->name,"comment")){
						node3 = node3->next;
					}
			  		value = xmlGetProp(node3, "value");			  
			  		strcpy(line,date);
			  		ptr = strtok(line, delimiter);
			  		fptr3 = fptr2;
			  	        if (isdigit(*ptr)){ 
			  	        	if (isdigit(*(ptr+1))){
			  	                	if (isdigit(*(ptr+2))){ 
			  	                  		if (isdigit(*(ptr+3))){
			  	                  			ptr = strtok(NULL, delimiter);
			  	                  			temp = strtod(ptr, &bla);
			  	                  			if ( 0 < temp && temp < 13 && isdigit(*(ptr+1)) && isdigit(*ptr) && strlen(ptr)==2 && ptr!=NULL){
			  	                  				ptr = strtok(NULL, delimiter);
			  	                  				temp = strtod(ptr, &bla);
			  	                  				if ( 0 < temp && temp < 32 && isdigit(*(ptr+1)) && isdigit(*ptr) && strlen(ptr)==2 && ptr!=NULL){
			  	                  					if (hour!=NULL && strtod(hour, &bla) < 25){
			  	                  						temp = strlen(value);
			  	                  						iter = 0;
			  	                  						if (isdigit(*(value))||*value =='-'){
				  	                  							++iter;
				  	                  							while(isdigit(*(value+iter))){
				  	                  								++iter;
				  	                  								if (iter >=temp) break;
				  	                  							}
				  	                  							if (*(value+iter)=='.'){
				  	                  								++iter;
				  	                  								if (iter >=temp) break;
				  	                  							}
				  	                  							while(isdigit(*(value+iter))){
				  	                  								++iter;
				  	                  								if (iter >=temp) break;
				  	                  							}
				  	                  							if (iter == temp) fptr3 = fptr;
			  	                  						}
			  	                  					}
			  	                  				}
			  	                  			}
			  	                  		}
			  	                  	}
			  	             	}
			  	        }
					fprintf(fptr3,"%s",date);
			  		fprintf(fptr3,"%s","; ");
			  		fprintf(fptr3,"%s", hour);
			  		fprintf(fptr3,"%s", "; ");
			  		fprintf(fptr3,"%s", value);
			  		fprintf(fptr3,"%s", "\n");
			  		node2 = node2->next;
		  		}
	  		}
	  		node1 = node1->next;	  			  
	  	}


	  }
	  node = node->next;  
  }
  xmlFreeDoc(doc);
  printf("Sucessfully converted the data of the XML file %s\n", argv[1]);
 
  xmlCleanupParser();
  return 1;
}
