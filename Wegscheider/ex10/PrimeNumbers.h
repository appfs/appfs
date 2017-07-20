/**
 * @file 	PrimeNummbers.h
 * @author 	Fabian Wegscheider
 * @date 	Jul 14, 2017
 */

#ifndef SRC_PRIMENUMBERS_H_
#define SRC_PRIMENUMBERS_H_

#include <vector>

using std::vector;

namespace PrimeNumbers {


	/**
	 * Finds all primes in {2,...,n} using the fact that if a natural number
	 * has a divisor it also has at least one prime divisor. the resulting vector
	 * contains the numbers -1.
	 * @param n the natural number up to which primes are generated
	 * @return a vector containing the prime numbers -1
	 */
	vector<int> findPrimes(int n);


}


#endif /* SRC_PRIMENUMBERS_H_ */
