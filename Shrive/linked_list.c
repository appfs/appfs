#include "ex1.h"

int print_linked_list(struct row *conductor)
{
	int count;
	printf("\n%d, %d, %lf\n", conductor->seq_no, conductor->location, conductor->value);
	while( conductor->next_row != NULL)
	{
		conductor = conductor->next_row;
		count++;
		printf("\n%d, %d, %lf\n", conductor->seq_no, conductor->location, conductor->value);
	}
	return 0;
}

int delete_linked_list (struct row *root)
{
//	printf("\n\tDeleteLinkedList\n\n");
	struct row *conductor = NULL;
	conductor =  malloc( sizeof(struct row) );
	if ( conductor )
	{
		conductor = root;
		while ( conductor->next_row != NULL)
		{
			root = conductor;
			free(root);
			conductor = conductor->next_row;
		}
	}
	else
	{
		printf("error: malloc returned null pointer\n");
		return 1;
	}
	free(conductor);
	return 0;
}
