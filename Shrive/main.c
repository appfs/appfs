#include "program.h"

int main(int argc, char * argv[])
{
	struct xy count;
	char * data = NULL;
	printf("\nImporting data...\n");
	data = read_data();
	count = count_data(data);
	geo_mean_data(data, count);
	free(data);
	printf("\nFin.\n");
	return 0;
}
