#include <cstdio>

using namespace std;

struct RGB {
	RGB() { R = 0; G = 0; B = 0; }

	unsigned char R, G, B;
};

struct cell {
	cell() { row = 0; col = 0; }
	cell( int COL, int ROW ) { col = COL; row = ROW; }

	int col, row;
};

int get_dir( int x1, int y1, int x2, int y2 ) {
	if ( x1 == x2 ) {
		if ( y1 < y2 ) { return 3; }
		else if ( y2 < y1 ) { return 1; }
	}
	else if ( y1 == y2 ) {
		if ( x1 < x2 ) { return 2; }
		else if ( x2 < x1 ) { return 0; }
	}
	return 4;
}

int main( int argc, char *argv[] ) {
	if ( argc < 2 || argc > 4 ) { printf( "usage: ./smazeimage maze.txt [path.txt]\n" ); return 1; }

	// Open and read maze.txt
	FILE *maze = fopen( argv[1], "r" ); int Nrow, Ncol;
	fscanf( maze, "MAZE %d %d", &Nrow, &Ncol );

	bool M[Ncol][Nrow][4]; int x1, y1, x2, y2;
	for ( int i = 0; i < Ncol; i++ ) {
		for ( int j = 0; j < Nrow; j++ ) {
			for ( int k = 0; k < 4; k++ ) {
				M[i][j][k] = false;
			}
		}
	}

	while( fscanf( maze, "%d %d %d %d", &y1, &x1, &y2, &x2 ) == 4 ) {
		M[x1][y1][ get_dir( x1, y1, x2, y2 ) ] = true;
		M[x2][y2][ get_dir( x2, y2, x1, y1 ) ] = true;
	}

	fclose( maze );

	// Open and write header to maze.ppm
	FILE *ppm = fopen( "maze.ppm", "wb" ); int var = 10;
	FILE *path; FILE *fppm;

	int size = ( 2 + ( Ncol - 1 ) + var * Ncol ) * ( 2 + ( Nrow - 1 ) + var * Nrow );
	int row = ( 2 + ( Nrow - 1 ) + var * Nrow ); int col = ( 2 + ( Ncol - 1 ) + var * Ncol );

	fprintf( ppm, "P6\n%d %d\n%d\n", col, row, 255 );

	// Create img array that holds image	
	RGB **img = new RGB *[row]; 
	img[0] = new RGB [size];

	for ( int i = 1; i < row; i++ ) {
		img[i] = img[i-1] + col;
	}

	// Make the entire image white
	for ( int i = 0; i < row; i++ ) {
		for ( int j = 0; j < col; j++ ) {
			img[i][j].R = 255; img[i][j].G = 255; img[i][j].B = 255;
		}
	}	

	if ( argc > 2 ) {
		// Open and read path.txt
		path = fopen( argv[2], "r" );
		fppm = fopen( "path.ppm", "wb" );
		fprintf( fppm, "P6\n%d %d\n%d\n", col, row, 255 );

		int g = 0; int h = 0; int psize = 0; 
		cell *p = new cell [ Nrow * Ncol ];
		fscanf( path, "PATH %d %d", &Nrow, &Ncol );

		while ( fscanf( path, "%d %d", &g, &h ) == 2 ) { 
			p[psize].row = g; p[psize].col = h;
			++psize;
		}

		// Color in the correct cells sea green
		int ans = 0; int r = 0; int c = 0; bool check = false;
		for ( int i = 1; i < row - 1; i += ( var + 1 ) ) {
			for ( int j = 1; j < col - 1; j += ( var + 1 ) ) {
				if ( p[ans].row == r && p[ans].col == c ) {
					for ( int a = i; a < ( i + var ); a++ ) {
						for ( int b = j; b < ( j + var ); b++ ) {
							img[a][b].R = 143; img[a][b].G = 188; img[a][b].B = 143;
						}
					} ++ans; i = -10; j = -10; c = 0; check = true; break;
				} ++c;
			} ++r; c = 0; if ( check ) { r = 0; check = false; }
		}
	}

	// Color in the maze lines | 0 = LEFT | 1 = UP | 2 = RIGHT | 3 = DOWN
	int r = 0; int c = 0;
	for ( int i = 0; i < col - 1; i += ( var + 1 ) ) {
		for ( int j = 0; j < row - 1; j+= ( var + 1 ) ) {
			for ( int k = 0; k < 4; k++ ) {
				if ( M[c][r][k] ) {
					if ( k == 0 ) {
						for ( int x = j; x < ( j + var + 2 ); x++ ) {
							img[x][i].R = 0; img[x][i].G = 0; img[x][i].B = 0;
						}
					}
					else if ( k == 1 ) {
						for ( int x = i; x < ( i + var + 2 ); x++ ) {
							img[j][x].R = 0; img[j][x].G = 0; img[j][x].B = 0;
						}
					}
					else if ( k == 2 ) {
						for ( int x = j; x < ( j + var + 2 ); x++ ) {
							img[x][i + var + 1].R = 0; img[x][i + var + 1].G = 0; img[x][i + var + 1].B = 0;	
						}
					}
					else if ( k == 3 ) {
						for ( int x = i; x < ( i + var + 2 ); x++ ) {
							img[j + var + 1][x].R = 0; img[j + var + 1][x].G = 0; img[j+ var + 1][x].B = 0;
						}
					}
				}
			} ++r;
		} ++c; r = 0;
	}

	// Color in the border
	for ( int i = 0; i < row; i++ ) {
		for ( int j = 0; j < col; j++ ) {
			if ( i == 0 && j > 11 ) {
				img[i][j].R = 0; img[i][j].G = 0; img[i][j].B = 0; }
			else if ( i == ( row - 1 ) && j < ( col - 12 ) ) {
				img[i][j].R = 0; img[i][j].G = 0; img[i][j].B = 0; }
			else if ( j == 0 && i > 0 ) {
				img[i][j].R = 0; img[i][j].G = 0; img[i][j].B = 0; }
			else if ( j == ( col - 1 ) && i < ( row - 1 ) ) {
				img[i][j].R = 0; img[i][j].G = 0; img[i][j].B = 0; }
		}
	}

	if( argc == 2 ) { fwrite( img[0], 3, size, ppm ); fclose( ppm ); }
	else{ fwrite( img[0], 3, size, fppm ); fclose( fppm ); }


	return 0;

}
