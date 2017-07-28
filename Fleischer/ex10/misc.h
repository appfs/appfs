/**
*@file misc.h
*@author Sven Fleischer
*There I've coded a function, which finds prime numbers 
*/

#ifndef MISC_H
#define MISC_H


/**
* Findes every prime from 2 to a given number.
*@param The number n, and the number of availabe threats.
*@return An array wich says if the index number is a prime or not.
*
*/
size_t* findPrimes(size_t n, int numThreads){
	
	omp_set_num_threads(numThreads);
	size_t* numbers = malloc(sizeof(size_t)*(n+1));
	numbers[0] = 0;
	numbers[1] = 0;

	
	#pragma omp for
	for (size_t i = 2; i <= n; ++i){
		numbers[i] = 1;
	}
	size_t pows = (size_t) pow((double) n, 0.5)+1;
	
	#pragma omp parallel
	#pragma omp for
	for (size_t i = 2; i < pows; ++i){
   		if(numbers[i]){
   			size_t num = 0;
   			size_t j = 0;
   			while((j = i*i+num*i) <= n){
   				numbers[j] = 0;
   				++num;
   			}
   		}
	} 
	return numbers;
}
#endif
