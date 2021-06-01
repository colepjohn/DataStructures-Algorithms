#include <fstream>
#include <iostream>
#include <string>
#include "PPM.h"

using namespace std;

void PPM::read( string &in ) { 
	ifstream fin;
	
	fin.open( in.c_str() );

	if ( fin.is_open() ) { 
		fin >> header >> ncols >> nrows >> maxval;
		
		if ( header != "P6" || maxval != 255 ) {
			cerr << "Invalid PPM file: Check header or maxvalue for pixels";
			return;
		}

		while ( fin.get() != '\n' );
	
		int Nrows = nrows;
		int Ncols = ncols;
		int size = nrows * ncols * 3;
		img = new RGB *[Nrows];
		img[0] = new RGB [Nrows*Ncols];

		for ( int i = 1; i < nrows; i++ ) {
			img[i] = img[i-1] + ncols;
		}
		
		fin.read( ( char * )img[0], size );

	}
	fin.close();
}

void PPM::write( string &out ) { 
	int sl = out.size();
	int size;
	out.insert( ( sl - 4 ), "_wmsg" );

	ofstream fout;
	fout.open( out.c_str() );

	if ( fout.is_open() ) {
		fout << header << '\n' << ncols << ' ' << nrows << '\n' << maxval << '\n';
		size = ncols * nrows * 3;

		fout.write( ( char * )img[0], size );

		fout.close();
	}
}
