#include <iostream>
#include "ex5b.h"
#include "ex5a.h"
#include <string>
#include <boost/timer/timer.hpp>

int main(int argc, char* argv[]){
	

	boost::timer::cpu_timer t;

	//checks if the input is right
	if (argc < 2 || argc > 3) {
			std::cerr << "wrong input" << std::endl;
			return 0;
	}


	if(argc==2){
		ex5b(argc,argv);
	}
	else if(strcmp(argv[2],"-m1")==0 || strcmp(argv[2],"-mod1")==0){
		ex5b(argc,argv);
	}
	else if(strcmp(argv[2],"-m2")==0 || strcmp(argv[2],"-mod2")==0){
		ex5a(argc,argv);
	}
	else{
		std::cout << "command \"" << argv[2] << "\" not found" << std::endl;
	}


 		
 		
	boost::timer::cpu_times time = t.elapsed();
 	std::cout <<"RESULT WALL-CLOCK TIME "<< time.wall*1e-9<<" seconds"<<std::endl;
 	std::cout <<"RESULT USER TIME "<< time.user*1e-9<<" seconds"<<std::endl;
	
}
