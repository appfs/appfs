/**
	@file
	@brief Main function for APPFS ex10. Reads in data, starts worker threads and outputs results.
	@author Tri-Peter Shrive
*/

#include "ex10.h"

int main(
	int argc,
	const char* const* const argv
	)
{
	printf( "\n" );
	if( 2 > argc || 4 < argc )
	{
		fprintf( stderr, "USAGE: %s *.gph ( number of terminals to start from ) ( -s )\n", argv[0] );
		exit( EXIT_FAILURE );
	}

	int i;
	int return_value = 0;
	unsigned int s_flag = 0;
	unsigned int max_num_start_terminals = 0;
	for( i = 2; i < argc; i++ )
	{
		// optionally print steiner tree
		if( 0 == strcmp( argv[i], "-s" ) ) 
			s_flag = 1;
		// set number of terminals to start from
		else if( 2 == i )
			return_value = sscanf( argv[i], "%u", &max_num_start_terminals );
	}
	// default is 100
	if( 1 != return_value )
		max_num_start_terminals = 100;
	
	// open file for reading
	FILE* fp = NULL;
	fp = fopen( argv[1], "r" );
	assert( fp );

	// read line
	unsigned int line_number = 0;
	char line[MAX_LINE_LENGTH];
	fgets( line, sizeof( line ), fp );
	line_number++;

	// remove comments and anything after newline
	char* character = NULL;
	character = strpbrk( line, "#\n" );
	if( NULL != character )
		*character = '\0';

	// skip initial spaces
	for( character = &line[0]; isspace( *character ); character++ );
	assert( '\0' != *character );

	unsigned int num_nodes;
	unsigned int num_edges;
	// read first line
	return_value = sscanf( character, "%u %u", &num_nodes, &num_edges );
	// undirected graph
	num_edges += num_edges;
	// node 0 may not exist
	num_nodes++;

	unsigned int array_next = 0;
	unsigned int array_length = EXTENSION_LENGTH;

	unsigned int* tails = NULL;
	unsigned int* heads = NULL;
	unsigned long long int* edge_weights = NULL;
	unsigned int* num_neighbours = NULL;
	tails = malloc( array_length * sizeof( unsigned int ) );
	heads = malloc( array_length * sizeof( unsigned int ) );
	edge_weights = malloc( array_length * sizeof( unsigned long long int ) );
	num_neighbours = calloc( num_nodes, sizeof( unsigned int ) );
	assert( tails );
	assert( heads );
	assert( edge_weights );
	assert( num_neighbours );

	unsigned int temp_tail = INT_MAX;
	unsigned int temp_head = INT_MAX;
	unsigned long long int temp_edge_weight = LLONG_MAX;
	while( NULL != fgets( line, sizeof(line), fp ) )
	{
		line_number++;
		// remove comments and anything after newline
		character = strpbrk( line, "#\n" );
		if( NULL != character )
		*character = '\0';

		// skip initial spaces
		for( character = &line[0]; isspace( *character ); character++ );
		// skip line if is empty
		if( '\0' == *character )
			continue;

		// read edge entries
		return_value = sscanf( character, "%u %u %llu", &temp_tail, &temp_head, &temp_edge_weight );
		if( 3 != return_value )
		{
			fprintf( stderr, "\nWARNING: line %u, sscanf returned %u != 3\n\n", line_number, return_value );
			continue;
		}
		if( num_nodes <= temp_tail )
		{
			fprintf( stderr, "\nWARNING: line %u, tail > number of nodes\n\n", line_number );
			continue;
		}
		if( num_nodes <= temp_head )
		{
			fprintf( stderr, "\nWARNING: line %u, head > number of nodes\n\n", line_number );
			continue;
		}
		if( 2000000000 <= temp_edge_weight )
		{
			fprintf( stderr, "\nWARNING: line %u, edge_weight >= 2000000000\n\n", line_number );
			continue;
		}
		// check there's enough space
		// if not enlarge array
		if( array_next == array_length )
		{
			array_length += EXTENSION_LENGTH;
			tails = realloc( tails, array_length * sizeof( unsigned int ) );
			heads = realloc( heads, array_length * sizeof( unsigned int ) );
			edge_weights = realloc( edge_weights, array_length * sizeof( unsigned long long int ) );
			assert( tails );
			assert( heads );
			assert( edge_weights );
		}
		// store edge attributes in memory
		tails[array_next] = temp_tail;
		heads[array_next] = temp_head;
		edge_weights[array_next] = temp_edge_weight;

		array_next++;
		num_neighbours[temp_tail]++;

		// repeat with head and tail inverted for undirected graph
		if( array_next == array_length )
		{
			array_length += EXTENSION_LENGTH;
			tails = realloc( tails, array_length * sizeof( unsigned int ) );
			heads = realloc( heads, array_length * sizeof( unsigned int ) );
			edge_weights = realloc( edge_weights, array_length * sizeof( unsigned long long int ) );
			assert( tails );
			assert( heads);
			assert( edge_weights );
		}
		// store reversed edge attributes in memory
		tails[array_next] = temp_head;
		heads[array_next] = temp_tail;
		edge_weights[array_next] = temp_edge_weight;

		array_next++;
		num_neighbours[temp_head]++;

	}
	assert( 0 == fclose( fp ) );
	assert( array_next == num_edges );

	// calculate index of first neighbour for sorted lists
	unsigned int j;
	unsigned int* first_neighbours_index = NULL;
	first_neighbours_index = calloc( num_nodes, sizeof( unsigned int ) );
	assert( first_neighbours_index );
	for( j = 1; j < num_nodes; j++ )
		first_neighbours_index[j] = first_neighbours_index[j - 1] + num_neighbours[j - 1]; 

	// sort heads and weights by tails
	unsigned int* num_neighbours_found = NULL;
	unsigned int* sorted_heads = NULL;
	unsigned int* sorted_tails = NULL;
	unsigned long long int* sorted_weights = NULL;
	num_neighbours_found = calloc( num_nodes, sizeof( unsigned int ) );
	sorted_heads = malloc( num_edges * sizeof( unsigned int ) );
	sorted_tails = malloc( num_edges * sizeof( unsigned int ) );
	sorted_weights = malloc( num_edges * sizeof( unsigned long long int ) );
	assert( num_neighbours_found );
	assert( sorted_heads );
	assert( sorted_tails );
	assert( sorted_weights );
	unsigned int k;
	unsigned int l;
	unsigned long long m;
	unsigned int index;
	for( j = 0; j < num_edges; j++ )
	{
		k = tails[j];
		l = heads[j];
		m = edge_weights[j];
		assert( k < num_nodes );
		assert( l < num_nodes );
		index = first_neighbours_index[k] + num_neighbours_found[k];
		sorted_weights[index] = m;
		sorted_heads[index] = l;
		sorted_tails[index] = k;
		num_neighbours_found[k]++;
	}
	free( tails );
	free( heads );
	free( edge_weights );


#ifndef NDEBUG
	// make sure we still have all edges 
	unsigned int total_neighbours = 0;

	for( j = 0; j < num_nodes; j++ )
	{
		total_neighbours += num_neighbours_found[j];
	}
	assert( num_edges == total_neighbours );
#endif	
	free( num_neighbours_found );

	// calculate prime numbers
	unsigned int* is_prime = NULL;
	is_prime = calloc( num_nodes, sizeof( unsigned int ) );
	assert( is_prime );
	unsigned int num_terminals = get_primes( is_prime, num_nodes );

	unsigned int* terminals = NULL;
	terminals = malloc( num_terminals * sizeof( unsigned int ) );
	assert( terminals );
	k = 0;
	for( j = 0; j < num_nodes; j++ )
		if( 1 == is_prime[j] )
		{
			terminals[k] = j;
			k++;
		}

	assert( k == num_terminals );

	printf( "NODES: %u\n", num_nodes );
	printf( "EDGES: %u\n", num_edges/2 );
	printf( "TERMINALS: %u\n",num_terminals );
	printf( "\n" );

	// we don't want to attempt to start from more terminals than there are
	if( num_terminals < max_num_start_terminals ) max_num_start_terminals = num_terminals;

	// start wall clock
	struct timeval start_wall;
	unsigned int fail = 1;
	fail = gettimeofday( &start_wall, NULL );
	assert( 0 == fail );
	// start counting cpu clocks
	double start_cpu = ( double ) clock() / ( double ) CLOCKS_PER_SEC;

	// calculate steiner tree
	unsigned int* tree = NULL;
	unsigned int* prev = NULL;
	tree = calloc( num_edges, sizeof( unsigned int ) );
	prev = malloc( num_nodes * sizeof( unsigned int ) );
	assert( prev );
	assert( tree );

	unsigned long long int obj_value = LLONG_MAX;

	unsigned int* temp_tree = NULL;	
	unsigned int* prev_edge_index = NULL;
	unsigned int* connected = NULL;

#ifdef THREADS
	#pragma omp parallel for default( none ) shared( prev, tree, obj_value, is_prime, terminals, num_terminals, max_num_start_terminals, first_neighbours_index, num_neighbours, num_nodes, num_edges, sorted_weights, sorted_heads, sorted_tails ) private( j, k, temp_tree, prev_edge_index ) num_threads( THREADS )
#endif
		for( j = 0; j < max_num_start_terminals; j++ )	
		{
			
			temp_tree = calloc( num_edges, sizeof( unsigned int ) );
			prev_edge_index = malloc( num_nodes * sizeof( unsigned int ) );
			assert( temp_tree );
			assert( prev_edge_index ); 

			unsigned int terminals_connected = 0;
			unsigned int source = terminals[j];
			terminals_connected = get_steiner_tree( is_prime, terminals, num_terminals, num_nodes, num_edges, first_neighbours_index, num_neighbours, sorted_weights, sorted_heads, sorted_tails, temp_tree, prev_edge_index, source );

			unsigned int temp_obj_value = 0;
			for( k = 0; k < num_edges; k++ )
			{
				if( 1 == temp_tree[k] )
				{
					temp_obj_value += sorted_weights[k];
				}
			}
#ifdef THREADS
			#pragma omp critical ( objective_value )
#endif
				if( temp_obj_value < obj_value && terminals_connected == num_terminals )
				{
					obj_value = temp_obj_value;
					memcpy( tree, temp_tree, num_edges * sizeof( unsigned int ) );
					memcpy( prev, prev_edge_index, num_nodes * sizeof( unsigned int ) );
				}

			free( temp_tree );
			free( prev_edge_index );
		}

	// stop wall clock
	struct timeval stop_wall;
	fail = 1;
	fail = gettimeofday( &stop_wall, NULL );
	assert( 0 == fail );
	// stop counting cpu clocks
	double stop_cpu = ( double ) clock() / ( double ) CLOCKS_PER_SEC;

	// calculate durations of both
	double duration_wall = ( stop_wall.tv_sec + stop_wall.tv_usec * 0.000001 ) -  ( start_wall.tv_sec + start_wall.tv_usec * 0.000001 );
	double duration_cpu = stop_cpu - start_cpu;


	if( s_flag )
	{
		printf( "TREE: " );
		for( j = 0; j < num_edges; j++ )
			if(  tree[j] )
				printf( "(%u,%u) ", sorted_tails[j], sorted_heads[j] );

		printf( "\n\n" );
	}

	printf( "TLEN: %llu\n", obj_value );
	printf( "TIME: %lf sec\n", duration_cpu );
	printf( "WALL: %lf sec\n", duration_wall );
	printf( "\n" );

	int tree_valid = 0;
	tree_valid = is_tree_valid( sorted_heads, sorted_tails, prev, terminals, num_terminals, num_nodes, num_edges);	
	( 1 == tree_valid ) ? printf( "TREE: TRUE\n" ) : printf( "TREE: FALSE\n" );
	printf( "\n" );

	free( tree );
	free( is_prime );
	free( terminals );
	free( first_neighbours_index );
	free( num_neighbours );
	free( sorted_weights );
	free( sorted_heads );
	free( sorted_tails );
	free( prev );

	return 0;
}
