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
	 * Computes all prime numbers in {2,...,n}. The numbers in the resulting
	 * vector are one lower than the actual prime number.
	 * @param n the natural number up to which primes are generated
	 * @return a vector containing the prime numbers -1
	 */
	vector<int> find_primes(int n);

}

#endif /* SRC_PRIMENUMBERS_H_ */
