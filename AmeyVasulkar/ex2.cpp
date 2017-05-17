//ex2 Amey Vasulkar
//This is a program to read an xmlfile given as an input in second argument.
//The program generates a text file in CSV format giving the file name in the start and the date in the form of- 
//YYYY-MM-DD;	HH;	amountOfPower-Value
//any errors can be seen on the console window.
//To run the program you need to have the below special header files saved in the folder where this file is saved.
//Tinyxml is used as a parser.
#include "tinyxml.h"
#include "tinystr.h"
#include "tinyxml.cpp"
#include "tinyxmlerror.cpp"
#include "tinyxmlparser.cpp"
#include "tinystr.cpp"
#include <iostream>
#include <cstdio>
#include <string>
#include <cstring>
#include <cstdlib>
#include <fstream>

int main(int argc, char *argv[])
{	
	// if file is not given as input the program exits.
	if (argc != 2)
	  std::cout<<"Please type the file name you want to read and try again."<<"\n";
	else
	{	

		std::ofstream outputFile;// Opening of a text file to write in the data.
		outputFile.open("data.txt");
		outputFile.clear();
		const char* pfilename= argv[1];// Loading the xml file.
		TiXmlDocument doc(pfilename);
		doc.LoadFile();
		bool loadOkay = doc.LoadFile();
		if (loadOkay)
		{
			std::string s (pfilename);		
			outputFile<<s<<"\n";
		}
		else 
		{	
			std::cout<<doc.ErrorDesc()<<"\n";
			return 0;
		}
		TiXmlHandle hDoc(&doc); //Requisite initialization to read the contents of the xml file.
		TiXmlElement* pElem;
		TiXmlHandle hRoot(0);
		pElem=hDoc.FirstChildElement("measured").Element();
			
		if (!pElem) 
		{
			std::cout<<doc.ErrorDesc()<<"\n";
			return 0;
		}
		
		hRoot=TiXmlHandle(pElem);			
		TiXmlElement* pgasDayNode=hRoot.FirstChildElement().Element();
		TiXmlHandle hRoot1(0);
		TiXmlHandle hRoot2 (0);
		
		//auto name=pboundaryNode->Value(); 
		//printf("\n%s:\n", name);
		for( pgasDayNode; pgasDayNode; pgasDayNode=pgasDayNode->NextSiblingElement()) // loops to go over the file and read the required content.
		{
			const char *pName=pgasDayNode->Attribute("date");	
			hRoot1=TiXmlHandle(pgasDayNode);				
			TiXmlElement* pboundaryNode=hRoot1.FirstChildElement().Element();
			
			for( pboundaryNode; pboundaryNode; pboundaryNode=pboundaryNode->NextSiblingElement())
			{
				hRoot2= TiXmlHandle(pboundaryNode);	
				TiXmlElement* ptimeNode=hRoot2.FirstChildElement().Element();
				
				for( ptimeNode; ptimeNode; ptimeNode=ptimeNode->NextSiblingElement())
				{
					const char *phour=ptimeNode->Attribute("hour");
					TiXmlElement* ppower=hRoot2.FirstChildElement().FirstChild().Element();
					const char *paof=ppower->Attribute("value");					
					
					if (pName!=0 && phour!=0 && paof!=0 ) 
					{
						std::string d (pName);
						std::string h (phour);
						std::string po (paof);
						outputFile<<d<<";"<<"\t"<<h<<";"<<"\t"<<po<<"\n"; //Reading the data in a text file.
					}
				}										
			}
				
		}	
		outputFile.close();
	}		
	return 0;
}