//include guard
#ifndef PROGRAM_H_
#define PROGRAM_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

struct xy {
	double x;
	double y;
};

int geo_mean_data(char * input, struct xy count);
struct xy count_data(char * input);
double nth_pow(double base, double n);
double nth_root(double base, double n);
char * read_data();

#endif
