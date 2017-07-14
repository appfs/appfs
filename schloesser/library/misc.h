 #ifndef MISC_H
 #define MISC_H
 
/** @file misc.h
 * @author Franziska Schlösser
 * @date July 2017
 * @brief Header file of misc.c
 */

/** @brief Define bool type
 */
typedef enum { false, true } bool;

/** @brief Returns a list of primes
 *
 * @param upper_bound upper bound, not included 
 * @return sieve number of primes found
 */
unsigned int* get_primes(unsigned int upper_bound);

#endif
