/** @file misc.c
 * @author Franziska Schlösser
 * @date July 2017
 * @brief Additional functions
 */

#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "misc.h"

unsigned int* get_primes(unsigned int upper_bound) {
    assert(upper_bound > 1);
    unsigned int *sieve = malloc(sizeof(*sieve) * upper_bound);
    for(int i = 0; i < upper_bound; i++) {
        sieve[i] = 1;
    }
    sieve[0] = 0; // 1 is not prime
    unsigned int bound = sqrt(upper_bound);
    for(int i = 2; i <= bound; i++) {
        if(sieve[i-1]==1) { // if i is prime
            // multiples of i are not prime
            unsigned int j = 2*i;
            while(j <= upper_bound) {
                sieve[j-1] = 0;
                j += i;
            }
       }
    }
    return sieve;
}

