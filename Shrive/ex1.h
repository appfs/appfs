//include guard
#ifndef EX1_H_
#define EX1_H_

#include <assert.h>
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 256

struct row {
	int seq_no, location;
	double value;
	struct row * next_row;
};

struct int_xy {
	int x, y;
};

struct double_xy {
	double x, y;
};

int print_linked_list(struct row *conductor);
int delete_linked_list (struct row *root);
struct double_xy geo_mean_rows(struct row *conductor, int count1, int count2);
struct int_xy get_lines(struct row * conductor, char * file_name);



#endif
