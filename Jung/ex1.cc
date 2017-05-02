#include <iostream>
#include <fstream>
#include <stack>
#include <cmath>
#include <time.h>
using namespace std;



void goToSemi(ifstream &myfile, int &i){             // stream goes to the next semicolon with it already extracted
    int where=myfile.get();
    while(where!=59){
        if(where==10){
            i++;
        }
        where=myfile.get();
    }
}



void goToNextLine(ifstream &myfile){                //stream goes to the next line
            int where = myfile.get();
             while(where!=10){                    // until the next line     10= int '\n'
                where=myfile.get();
            }
}




int goToLoc(ifstream &myfile){                          // stream goes to the next number with it already extracted and gives back its value
                                                        // if there is no number before the next semicolon it returns 0 (and semicolon is extractet) 
    int where = myfile.get();
    int where2=where;
    while ((where<48 || where > 57)&&where!=59){
        
        where = myfile.get();
    }
    if(where==59){
        where=48;
    }
    return where-48;
}




void addNumber(ifstream &myfile, long double &w, long int n, int &i){                        //extracts the next number and calculates the actual geometric mean given the location 
    
    long double v=0.0;
    int where = myfile.get();
    int count = 0;
    int point =0;
    stack<int> number;
    
    while (where<48 || where > 57){
        if(where==10){
            i++;
        }
        where = myfile.get();
    }
    
    while ((where>=48&&where<=57)||where==46){
        
        if(where==46){
            point=count;
        }
        else{
            count++;
            number.push(where-48);
        }
        where=myfile.get();
    }
        myfile.unget();
        
        
    for(int j=count; j>0; j--){
        v=v+(number.top())*(pow(10,(point-j)));
        number.pop();
    }
    if(w==0){
        w=v;
    }
    else{
        long double exponent=(1+(log(1/w))/(log(v)))/(n+1);
        w=w*pow(v,exponent);
    }
    

    
}





void mean(long double * means){
    
    ifstream myfile;
    myfile.open("ex1.dat");
    
    //cout<<myfile.eof()<<endl;
    int i=1;
    int where=myfile.get();
    int loc;
    long int loc1=0;
    long int loc2=0;
    
    while(where!=-1){

        if(where==35){                  // ignore lines with #
            
            where=myfile.get();
            
            while(where!=10 && where!=-1){       // goes to next line
               where=myfile.get();
            }
            i++;
            where=myfile.get();
        }
        
        
        else{
            
            goToSemi(myfile,i);
            loc=goToLoc(myfile);
            goToSemi(myfile,i);
            
            
            if(loc==1){
                addNumber(myfile,means[0],loc1, i);  
                loc1++;
            }
            else if(loc==2){
                addNumber(myfile,means[1],loc2, i);
                loc2++;
            }
            
            else{

            }

            goToNextLine(myfile);
            where=myfile.peek();
            i++;

        }
    
        
    }
    
    cout<< "File: ex1.dat with :"<< i << "  lines"<< endl;
    cout<<" Valid Values Loc1:"<< loc1<< "  With GeoMean: "<<means[0]<<endl;
    cout<<" Valid Values Loc1:"<< loc2<< "  With GeoMean: "<<means[1]<<endl;
    
}







int main(){
    clock_t tStart = clock();
    ifstream myfile;
    myfile.open("ex1.dat");


    
    long double means[2]={0.0,0.0};
    mean(means);
    
    cout<<"Time needed: ";
    cout<<(double)(clock()-tStart)/CLOCKS_PER_SEC<<" seconds"<<endl;
}
