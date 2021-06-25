#include <cstdlib>
#include <ctime>
#include <cstdio>

#include "dset.h"

struct cell {
	cell() { row = 0; col = 0; }
	cell( int i, int j ) { row = i; col = j; }

	int row, col;
};

int main( int argc, char *argv[] ) {
	if ( argc != 4 ) {
		// print usage error message to stderr
		printf( "usage: ./Mazemake nrow ncol maze.txt\n" );

		return 0;
	}

	int Nrow = atoi( argv[1] );
	int Ncol = atoi( argv[2] );

	// Grid wall:     (i0,j0) | (i1,j1)
	// Horizontal wall: (i,j) | (i+1,j)
	// Vertical wall:   (i,j) | (i,j+1)

	// int N = number of interior grid walls
	int N = ( ( Ncol - 1 ) * Nrow ) + ( ( Nrow - 1 ) * Ncol );
	cell wall[N][2];

	// populate wall[][] with interior grid walls
	int k = 0;
	for ( int i = 0; i < Nrow - 1; i++ ) {
		for ( int j = 0; j < Ncol; j++ ) {
			wall[k][0] = cell( i, j );
			wall[k][1] = cell( i + 1, j );
			k++;
		}
	}
	for ( int i = 0; i < Nrow; i++ ) {
		for ( int j = 0; j < Ncol - 1; j++ ) {
			wall[k][0] = cell( i, j );
			wall[k][1] = cell( i, j + 1 );
			k++;
		}
	}

	// randomly perturb list order: swap based
	srand( time( NULL ) );
	for ( int i = N - 1; i > 0; --i ) {
		// swap( &wall[i], &wall[ rand() % ( i + 1 ) ] );
		int rd = rand() % ( i + 1 );  cell tmp[2];
		tmp[0] = wall[i][0];		  tmp[1] = wall[i][1];
		wall[i][0] = wall[rd][0];     wall[i][1] = wall[rd][1];
		wall[rd][0] = tmp[0];		  wall[rd][1] = tmp[1];

	}

	// open output file for writing (argv[3])
	FILE * file = fopen( argv[3], "w" );

	// write MAZE, Nrow, Ncol header 
	fprintf( file, "MAZE %d %d\n", Nrow, Ncol );

	dset ds( Nrow * Ncol );

	for ( k = 0; k < N; k++ ) {
		int ii = wall[k][0].col + ( wall[k][0].row * Ncol );
		int jj = wall[k][1].col + ( wall[k][1].row * Ncol );
		
		// if: pair of cells given by wall[k][] are not
		// connected (belong to different disjoint sets),
		// merge them
		if ( ds.find( ii ) != ds.find( jj ) )
			ds.merge( ii, jj );

		// else: write wall location to file as two pairs 
		// of cell indices: i0 j0 i1 j1
		else
			fprintf( file, "%*d %*d %*d %*d\n", 3, wall[k][0].row, 4, wall[k][0].col, 4, wall[k][1].row, 4, wall[k][1].col );

		// if: all cells belong to the same set, break
		if ( ds.size() == 1 )
			break;
	} k++;

	// write any remaining walls to file
	for ( ; k < N; k++ )
		fprintf( file, "%*d %*d %*d %*d\n", 3, wall[k][0].row, 4, wall[k][0].col, 4, wall[k][1].row, 4, wall[k][1].col );

	// close output file
	fclose( file );
}
