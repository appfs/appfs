/**
 * @file 	PrimeNumbers.cpp
 * @author 	Fabian Wegscheider
 * @date 	Jul 14, 2017
 */

#include "PrimeNumbers.h"



vector<int> PrimeNumbers::findPrimes(int n) {

	vector<int> primes;
	if (n >= 2) primes.push_back(2);

	for (int i = 3; i < n; ++i) {
		bool isPrime = true;
		for (unsigned int j = 0; j < primes.size() && primes[j]*primes[j] <= i; j++) {
			if (i % primes[j] == 0) {
				isPrime = false;
				break;
			}
		}
		if (isPrime) {
			primes.push_back(i);
		}
	}

	for (unsigned int i = 0; i < primes.size(); ++i) {
		primes[i]--;
	}

	return primes;
}





