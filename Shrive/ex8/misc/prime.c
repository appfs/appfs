#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>

int main(int argc, const char* const* const argv)
{
	assert( 2 == argc);

	int max;
	assert( 1== sscanf( argv[1], "%d", &max) );

	int rest;
	int * prime;
	prime = calloc( max, sizeof(int) );

	printf("\n2\t");
	int count = 1;
	int last_div;
	for ( int i = 3; i <= max; i += 2 )
	{
		last_div =  (int)ceil(sqrt(i));
		for ( int j = 2; j <= last_div ; j++ )
		{
			rest = i % j;
			if ( 0 == rest )
				break;

			if ( j == last_div )
				prime[i] = 1;
		}
		if ( 1 == prime[i] )
		{
			count++;
			printf("%d\t", i);
			if ( 0 == count % 10 )
				printf("\n");
		}
	}
	printf("\n\n");
	free( prime );
	return 0;
}
