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

void set_pixel_list( PPM &img1, PPM &img2, vector<pixel> &pixel_list, int seed ) {
	int nrows = img2.get_Nrows();
	int ncols = img2.get_Ncols();
	int size = nrows * ncols;
	pixel_list.resize( size );
	
	size = img1.get_Nrows() * img1.get_Ncols();

	unsigned int index = 0;

	for ( int i = 0; i < nrows; i++ ) {
		for ( int j = 0; j < ncols; j++ ) {
			pixel_list[index].row = i;
			pixel_list[index].col = j;
			++index;
		}
	}

	index = 0; vector<int> hist ( 4096, 0 );

	while ( index < size ) {
		int shift = 12; unsigned char c = 0x0;
		for ( int t = 0; t < 3; t++ ) {
			for ( int j = 6; j >= 3; j-- ) {
				if ( t == 0 ) {
					unsigned char x = img1[pixel_list[index].row][pixel_list[index].col].R;
					x = ( x >> j ); x &= 0x1; c |= ( x << shift );
				}
				else if ( t == 1 ) {
					unsigned char x = img1[pixel_list[index].row][pixel_list[index].col].G;
					x = ( x >> j ); x &= 0x1; c |= ( x << shift );
				}
				else if ( t == 2 ) {
					unsigned char x = img1[pixel_list[index].row][pixel_list[index].col].B;
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

void encode( PPM &img, vector<pixel> pixel_list, string key ) {
	char c; unsigned int j = 0; unsigned int letter = 0;
	while ( cin.get( c ) ) {
		if ( letter == key.size() ) { letter = 0; }
		c ^= key.at( letter ); ++letter;

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

void decode( PPM &img, vector<pixel> pixel_list, string key ) {
	unsigned char x = 0x0; unsigned int j = 0; unsigned int letter = 0;
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
		if ( letter == key.size() ) { letter = 0; }
		x ^= key.at(letter); ++letter;
		if ( x != ETX ) { cout.put( x ); x = 0x0; }
		else { break; }
	}
}

void set_encryption_key ( int seed, string &key ) {
	vector<int> hist ( 16, 0 );
	
	for ( unsigned int i = 0; i < key.size(); i++ ) {
		unsigned char x = key.at(i); unsigned char c = 0x0;
		for ( int j = 3; j >= 0; j-- ) {
			x = ( x >> j ); x &= 0x1; c |= ( x << j );
		}
		++hist[( int )c];
	}
	
	rnumgen KEY( seed, hist );

	for ( unsigned int i = key.size() - 1; i > 0; --i ) {
		swap( key[i], key[ KEY.rand() % ( i + 1 ) ] );
	}
}

int main(int argc, char *argv[]) {
	/*  parse command line arguments
		if something not right,
		print error message and exit
		*/
	int num;
	string mode, seed, fname1, fname2, def, key;
	if ( argc != 5 && argc != 6 ) {
		cerr << "Invalid number of arguments given: Expected 5 or 6, given " << argc << '\n';
		return 1;
	}
	else {
		mode = argv[1];
		if ( mode != "-encode" && mode != "-decode" ) {
			cerr << "Invalid command: Expected -encode or -decode, given " << mode << '\n';
			return 1;
		}
		seed = argv[2]; def = seed; def.erase( 0, 6 ); num = atoi( def.c_str() ); seed.resize(6);
		if ( seed != "-seed=" ) {
			cerr << "Invalid argument: Expected -seed= followed by an integer, given " << seed << '\n';
			return 1;
		}
		if ( argc == 6 ) {
			fname1 = argv[4]; fname2 = argv[5]; key = argv[3]; key.erase( 0, 5 );
		}
		else { 
			fname1 = argv[3]; fname2 = argv[4]; key = "GoVols!";
		}
	}

	PPM img1; PPM img2;
	img1.read( fname1 );
	img2.read( fname2 );

	//declare pixel_list
	vector<pixel> pixel_list;
	set_pixel_list( img1, img2, pixel_list, num );

	set_encryption_key( num, key );
	
	if ( mode == "-encode" ) {
		encode ( img2, pixel_list, key );
		img2.write( fname2 );
	}
	else if ( mode == "-decode" ) {
		decode ( img2, pixel_list, key );
	}
}
