#include "ex1.h"

int main(int argc, char * argv[])
{
	assert(argc > 1);
	char * file_name = argv[1];

	struct row * root = NULL;
	root = calloc( 1, sizeof(struct row));
	root->next_row = NULL;

	struct int_xy count;
	struct double_xy geo_mean;

	printf("\nImporting data.\n");

	count = get_lines( root, file_name );
	
//	print_linked_list(root);

	printf("\nCalculating geometric mean.\n");

	geo_mean = geo_mean_rows( root, count.x, count.y );

	printf("\nCount 1 = %d, geometric mean 1 = %f.\n", count.x, geo_mean.x);
	printf("\nCount 2 = %d, geometric mean 2 = %f.\n", count.y, geo_mean.y);

	delete_linked_list( root );

	printf("\nFin.\n\n");
	return 0;
}
