#include <string>
#include <iostream>
#include <stdlib.h>
#include <cmath>

using namespace std;

#include "PPM.h"
#include "Rnumgen.h"

const char ETX = 0x3;

struct pixel {
	int row, col;
};

void set_pixel_list( PPM &img, vector<pixel> &pixel_list, int seed ) {
	int nrows = img.get_Nrows();
	int ncols = img.get_Ncols();
	int size = nrows * ncols;
	pixel_list.resize( size );

	unsigned int index = 0;

/*	for ( int i = 0; i < nrows; i+=2 ) {
	    for ( int j = 0; j < ncols; j += 2 ) {
			pixel_list[index].row = i;
			pixel_list[index].col = j;
			++index;
		}
	}*/
	for ( int i = 0; i < nrows; i++ ) {
		for ( int j = 0; j < ncols; j++ ) {
			pixel_list[index].row = i;
			pixel_list[index].col = j;
			++index;
		}
	}

	index = 0; vector<int> hist ( 4096, 0 );
	
	while ( index < pixel_list.size() ) {
		int shift = 12; unsigned char c = 0x0;
		for ( int t = 0; t < 3; t++ ) {
			for ( int j = 6; j >= 3; j-- ) {
				if ( t == 0 ) {
					unsigned char x = img[pixel_list[index].row][pixel_list[index].col].R;
					x = ( x >> j ); x &= 0x1; c |= ( x << shift );
				}
				else if ( t == 1 ) {
					unsigned char x = img[pixel_list[index].row][pixel_list[index].col].G;
					x = ( x >> j ); x &= 0x1; c |= ( x << shift );
				}
				else if ( t == 2 ) {
					unsigned char x = img[pixel_list[index].row][pixel_list[index].col].B;
					x = ( x >> j ); x &= 0x1; c |= ( x << shift );
				}
				--shift;
			}
		}
		++index; ++hist[( int )c];
	}
	
	rnumgen RNG( seed, hist );

	for ( int i = pixel_list.size() - 1; i > 0; --i ) {    
		unsigned int r1_12 = RNG.rand();
		unsigned int r2_12 = RNG.rand();
		unsigned int r_24 = ( ( r1_12 << 12 ) | r2_12 );
		swap( pixel_list[i], pixel_list[ r_24 % ( i + 1 ) ] );
	}
}

void encode( PPM &img, vector<pixel> pixel_list ) {
	char c;
	int j = 0;
	while ( cin.get( c ) ) {
		for ( int i = 0; i < 7; i++ ) {
			if ( ( j % 3 ) == 0 ) {
				img[pixel_list[j].row][pixel_list[j].col].R &= 0xFE;
				img[pixel_list[j].row][pixel_list[j].col].R |= ( ( c >> i ) & 0x1 );
			}
			if ( ( j % 3 ) == 1 ) {
				img[pixel_list[j].row][pixel_list[j].col].G &= 0xFE;
				img[pixel_list[j].row][pixel_list[j].col].G |= ( ( c >> i ) & 0x1 );
			}
			if ( ( j % 3 ) == 2 ) {
				img[pixel_list[j].row][pixel_list[j].col].B &= 0xFE;
				img[pixel_list[j].row][pixel_list[j].col].B |= ( ( c >> i ) & 0x1 );
			}
			++j;
		}
	}
	for ( int i = 0; i < 7; i++ ) {
		if ( ( j % 3 ) == 0 ) {
			img[pixel_list[j].row][pixel_list[j].col].R &= 0xFE;
			img[pixel_list[j].row][pixel_list[j].col].R |= ( ( ETX >> i ) & 0x1 );
		}
		if ( ( j % 3 ) == 1 ) {
			img[pixel_list[j].row][pixel_list[j].col].G &= 0xFE;
			img[pixel_list[j].row][pixel_list[j].col].G |= ( ( ETX >> i ) & 0x1 );
		}
		if ( ( j % 3 ) == 2 ) {
			img[pixel_list[j].row][pixel_list[j].col].B &= 0xFE;
			img[pixel_list[j].row][pixel_list[j].col].B |= ( ( ETX >> i ) & 0x1 );
		}
		++j;
	}
}

void decode( PPM &img, vector<pixel> pixel_list ) {
	unsigned char x = 0x0; int j = 0;
	while ( 1 ) {
		for ( int i = 0; i < 7; i++ ) {
			if ( ( j % 3 ) == 0 ) {
				unsigned char c = img[pixel_list[j].row][pixel_list[j].col].R;
				c &= 0x1; c = c << i; x |= c;
			}
			else if ( ( j % 3 ) == 1 ) {
				unsigned char c = img[pixel_list[j].row][pixel_list[j].col].G;
				c &= 0x1; c = c << i; x |= c;
			}
			else if ( ( j % 3 ) == 2 ) {
				unsigned char c = img[pixel_list[j].row][pixel_list[j].col].B;
				c &= 0x1; c = c << i; x |= c;
			}
			++j;
		}
		x |= ( 0x0 << 7 );
		if ( x != ETX ) { cout.put( x ); x = 0x0; }
		else { break; }
	}
}

int main(int argc, char *argv[]) {
	/*  parse command line arguments
		if something not right,
		print error message and exit
		*/
	int num;
	string mode, seed, fname, def;
	if ( argc != 3 && argc != 4 ) {
		cerr << "Invalid number of arguments given: Expected 3 or 4, given " << argc << '\n';
		return 1;
	}
	else {
		mode = argv[1];
		if ( mode != "-encode" && mode != "-decode" ) {
			cerr << "Invalid command: Expected -encode or -decode, given " << mode << '\n';
			return 1;
		}
		if ( argc == 4 ) {
			seed = argv[2]; def = seed; def.erase( 0, 6 ); num = atoi( def.c_str() ); seed.resize(6); fname = argv[3];
			if ( seed != "-seed=" ) {
				cerr << "Invalid argument: Expected -seed= followed by an integer, given " << seed << '\n';
				return 1;
			}
		}
		else { num = 0; fname = argv[2]; }
	}

	PPM img;
	img.read(fname);

	//declare pixel_list
	vector<pixel> pixel_list;
	set_pixel_list( img, pixel_list, num );
	
	if ( mode == "-encode" ) {
		encode ( img, pixel_list );
		img.write( fname );
	}
	else if ( mode == "-decode" ) {
		decode ( img, pixel_list );
	}
}
