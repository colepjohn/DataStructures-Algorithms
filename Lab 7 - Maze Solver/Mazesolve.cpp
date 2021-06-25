#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>

using namespace std;

struct cell {
	cell() { x = 0; y = 0; }
	cell( int X, int Y ) { x = X; y = Y; }

	int x, y;
};

int get_dir( int x1, int y1, int x2, int y2 ) {
	if ( x1 == x2 ) {
		if ( y1 < y2 ) { return 3; }
		else if ( y2 < y1 ) { return 1; }
	}
	else if ( y1 == y2 ) {
		if ( x1 < x2 ) { return 2; }
		else if ( x2 < x1 ) { return 0; }
	} return 4;
}

int main(int argc, char *argv[]) {
	if (argc != 3) {
		// print usage error message to stderr
		printf( "usage: ./Mazesolve maze.txt path.txt\n" );
		
		return 0;
	} 

	int Nrow, Ncol;

	// open input file for reading
	// open output file for writing
	FILE * fin  = fopen( argv[1], "r" );
	FILE * fout = fopen( argv[2], "w" );

	// determine Nrow,Ncol from input file
	fscanf( fin, "MAZE %d %d", &Nrow, &Ncol );

	// create array of walls for each grid cell
	// initialize to have boundary walls set and
	// interior walls unset

	bool M[Ncol][Nrow][4];
	for ( int i = 0; i < Ncol; i++ ) {
		for ( int j = 0; j < Nrow; j++ ) {
			for ( int k = 0; k < 4; k++ ) {
				M[i][j][k] = false;
			}
		}
	}
	for ( int i = 0; i < Ncol; i++ ) {
		for ( int j = 0; j < Nrow; j++ ) {
			if ( i == 0 ) { M[i][j][0] = true; }
			if ( i == Ncol - 1 ) { M[i][j][2] = true; }
			if ( j == 0 ) { M[i][j][1] = true; }
			if ( j == Nrow - 1 ) { M[i][j][3] = true; }
		}
	}

	// read input file, set interior walls
	int y1, y2, x1, x2;
	while( fscanf( fin, "%d %d %d %d", &y1, &x1, &y2, &x2 ) == 4 ) {
		M[x1][y1][ get_dir( x1, y1, x2, y2 ) ] = true;
		M[x2][y2][ get_dir( x2, y2, x1, y1 ) ] = true;
	}

	// initalize DFS path computation
	cell *stack = new cell [ Nrow * Ncol ]; int stack_size;
	// DIR_T *D = new DIR_T [ Nrow * Ncol ];
	int *D = new int [ Nrow * Ncol ];

	// source = ( 0, 0 )
	// sink = ( Nrow - 1, Ncol - 1 )
	cell S; S.y = 0; S.x = 0;
	cell T; T.y = Nrow - 1; T.x = Ncol - 1;

	int V[Ncol][Nrow];
	for ( int i = 0; i < Ncol; i++ ) {
		for ( int j = 0; j < Nrow; j++ ) {
			V[i][j] = 0;
		}
	}
	
	// carry out DFS source-to-sink computation
	cell point, to;
	stack[0].y = S.y; stack[0].x = S.x;
	D[0] = 0;
	V[S.x][S.y] = true;
	stack_size = 1;
	int tmp = stack_size;

	while ( stack_size > 0 ) {
		if ( stack_size != tmp ) {
			V[ stack[ tmp ].x ][ stack[ tmp ].y ] = true;
			tmp = stack_size;
		}

		while ( D[ stack_size - 1 ] == 4 ) {
			V[ stack[ stack_size ].x ][ stack[ stack_size ].y ] = false;
			stack_size -= 1;
			
			if ( stack_size == 0 ) break;
		}
		
		point.x = stack[ stack_size - 1 ].x; point.y = stack[ stack_size - 1 ].y;
		
		if ( point.y == T.y && point.x == T.x ) break;
		
		int dir = D[ stack_size - 1];
		switch ( dir ) {
			case 0:
				to.y = point.y;
				to.x = point.x - 1;
				break;
			case 1:
				to.y = point.y - 1;
				to.x = point.x;
				break;
			case 2:
				to.y = point.y;
				to.x = point.x + 1;
				break;
			case 3:
				to.y = point.y + 1;
				to.x = point.x;
				break;
		}
		
		D[ stack_size - 1 ] += 1;
		
		if ( M[ point.x ][ point.y ][ D[ stack_size - 1 ] - 1 ] ) continue;
		
		if ( V[ to.x ][ to.y ] == 1 ) continue;
		
		stack[ stack_size ].y = to.y; stack[ stack_size ].x = to.x;
		D[ stack_size ] = 0;
		V[ to.x ][ to.y ] = true;
		stack_size += 1;
	}

	// write PATH, Nrow, Ncol header
	fprintf( fout, "PATH %d %d\n", Nrow, Ncol ); 
	// write cells on path to file
	for ( int i = 0; i < stack_size; i++ ) {
		fprintf( fout, "%d %d\n", stack[i].y, stack[i].x );
	}
	
	// close open files
	fclose( fin );
	fclose( fout );

	return 0;
}
