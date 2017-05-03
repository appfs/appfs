/**@file   ex1.c
 * @brief  Appfs Exercise 1: reading csv.
 * @author Thorsten Koch
 * @date   16Apr2017
 *
 * gcc -std=c99 -O3 -Wall -Wextra -Wpedantic ex1a.c -o ex1a -lm
 * cppcheck --std=c99 --enable=all ex1a.c
 *
 * Using fgets for input
 */  
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <inttypes.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <fenv.h>

//#include "mshell.h"

#define EXT_SIZE     (1024*1024)
#define MAX_LINE_LEN 512
#define LOCS         2

#define error_exit(msg)  error_exit_fun(msg, __FILE__, __LINE__)

// Note: The constant string is put several times into the code, but the compiler will automatically combine.
#define print_error_and_continue(ln, lb)  \
  { fprintf(stderr, "%s(%d): Line %" PRIuFAST32 ": Syntax error [%s]\n", __FILE__, __LINE__, ln, lb); continue; }

/** Print error message with source file name and line number
 */
static void error_exit_fun(const char* const msg, const char* const file, const int lineno)
{
   assert(NULL != msg);
   assert(NULL != file);

   fprintf(stderr, "%s(%d) ", file, lineno);
   perror(msg);
   exit(EXIT_FAILURE);
}

#if 1 
/** Compute geometric mean of vector
 * log based version of computing the geometric mean
 * exp( 1/n * sum_i=1..n log(x_i) )
 */
static double geom_mean_log(const double* const x, const size_t count)
{
   assert(NULL != x);
   
   double sum = 0.0;

   // We do this as an asswert, because we do not expect an error to be possible
   assert(!feclearexcept(FE_ALL_EXCEPT));
      
   for(size_t i = 0; i < count; i++) // note: due to unsigned  while(count >= 0) does not work
   {
      assert(!isnan(x[i]) && !islessequal(x[i], 0.0));

      sum += log2(x[i]); 
   }
   double gm = exp2(sum / (double)count);

   // We do this as an asswert, because we do not expect an error to be possible. Or?
   assert(!fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW));

   return gm;
}

#else
/** Compute geometric mean of vector
 * pow based version of computing the geometric mean
 */
static double geom_mean_pow(const double* const x, const size_t count)
{
   assert(NULL != x);
   
   double gm     = 1.0;
   double factor = 1.0 / (double)count;
   
   // We do this as an asswert, because we do not expect an error to be possible
   assert(!feclearexcept(FE_ALL_EXCEPT));
      
   for(size_t i = 0; i < count; i++) // note: due to unsigned  while(count >= 0) does not work
   {
      assert(!isnan(x[i]) && !islessequal(x[i], 0.0));

      gm *= pow(x[i], factor); 
   }
   // We do this as an asswert, because we do not expect an error to be possible. Or?
   assert(!fetestexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW));

   return gm;
}
#endif 

/* Read file with format:
 * 765; 3; 34.786
 * # is comment, empty lines are allowed
 */
int main(int argc, const char* const* const argv)
{   
   // Check arguments, we need a filename.
   if (argc < 2)
   {
      fprintf(stderr, "usage: %s filename\n", argv[0]);
      exit(EXIT_FAILURE);
   }
   size_t  loc_next  [LOCS];  // the index of the next number to store
   size_t  loc_size  [LOCS];  // the size of the arrays to store the numbers
   double* loc_value [LOCS];  // array to store the numbers

   for(uint_fast8_t i = 0; i < LOCS; i++)
   {
      loc_next [i] = 0;         // we start empty 
      loc_size [i] = EXT_SIZE;  // initial size
      loc_value[i] = malloc(loc_size[i] * sizeof(loc_value[i][0]));

      if (NULL == loc_value[i])
         error_exit("malloc: ");      
   }
   
   // Open file for reading
   FILE* fp = fopen(argv[1], "r");

   if (NULL == fp)
      error_exit("fopen: ");

   uint_fast32_t lineno = 0;
   char          line[MAX_LINE_LEN];

   // Read line by line
   while (NULL != fgets(line, sizeof(line), fp))
   {
      lineno++;

      // Remove comments and newline
      char* s = strpbrk(line, "#\n");

      if (NULL != s)
         *s = '\0';

      // Skip initial spaces
      for (s = &line[0]; isspace(*s); s++)
         ;

      // If line turns out empty, skip it
      if ('\0' == *s)
         continue;

      uint8_t loc   = 0;
      double  value = NAN;

#if 0 // Short but slow

      // We can ignore the sequence no. Just get the location and the value
      if (2 != sscanf(s, "%*d; %" SCNu8 "; %lf", &loc, &value))
         print_error_and_continue(lineno, line);

      if (loc < 1 || loc > LOCS)
         print_error_and_continue(lineno, line);
#else
      // Skip over Sequence number         
      while(isdigit(*s))
         s++;

      while(isspace(*s))
         s++;

      if (';' != *s)
         print_error_and_continue(lineno, line);

      // Get Loc number
      s++;
      while(isspace(*s))
         s++;

      if (*s < '1' || *s > (LOCS + '0'))
         print_error_and_continue(lineno, line);
      
      loc = *s - '0';
      
      s++;      
      if (isspace(*s))
         s++;
      
      if (';' != *s)
         print_error_and_continue(lineno, line);

      // Get Value
      s++;

      char* ep = NULL;
      
      value = strtod(s, &ep);

      // Nothing converted
      if (s == ep)
         print_error_and_continue(lineno, line);

      while(isspace(*ep))
         ep++;		

      // Check for extra data after the value
      if ('\0' != *ep)
         print_error_and_continue(lineno, line);          
#endif
      
      if (!isnormal(value) || value <= 0.0)
      {         
         fprintf(stderr, "Line %" PRIuFAST32 ": Invalid value\n", lineno);
         continue;
      }
      loc--;

      assert(loc < LOCS); // Explain signed/unsigned regarding "--" on zero before 

      // check we have enough space in the array, otherwise enlarge the array
      if (loc_next[loc] == loc_size[loc])
      {
         loc_size [loc] += EXT_SIZE;
         loc_value[loc] = realloc(loc_value[loc], loc_size[loc] * sizeof(loc_value[loc][0]));

         if (NULL == loc_value[loc])
            error_exit("realloc: ");
      }
      assert(loc_next[loc] < loc_size[loc]);

      // finally, store the value
      loc_value[loc][loc_next[loc]] = value;
      loc_next[loc]++;
   }
   if (fclose(fp))
      error_exit("fclose: ");

   printf("File: %s with %" PRIuFAST64 " lines\n", argv[1], lineno);

   // print statistics and free space
   for(uint_fast8_t i = 0; i < LOCS; i++)
   {
      printf("Valid values Loc%" PRIuFAST8 ": %zu with GeoMean: %.6f\n",
         i, loc_next[i], geom_mean_log(loc_value[i], loc_next[i]));

      free(loc_value[i]);

   }
   return EXIT_SUCCESS;
}

