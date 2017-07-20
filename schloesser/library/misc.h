 #ifndef MISC_H
 #define MISC_H
 
/** @file misc.h
 * @author Franziska Schlösser
 * @date July 2017
 * @brief Header file of misc.c
 */

/** @brief Returns a mask indicating primes.
 * Computation via the sieve method
 *
 * @param upper_bound upper bound, not included 
 * @return number of primes found
 */
unsigned int* get_primes(unsigned int upper_bound);

#endif
