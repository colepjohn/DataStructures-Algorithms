#ifndef __PPM_H__
#define __PPM_H__

#include <cstddef>
#include <string>

struct RGB { 
  RGB() { R = 0; G = 0; B = 0; }

  unsigned char R, G, B;
};

class PPM { 
  public:
    PPM() { nrows = 0; ncols = 0; }
    ~PPM() { delete img; }

	void read( std::string & );
	void write( std:: string & );

	RGB *operator[]( int i ) { return img[i]; }

	int get_Nrows() { return nrows; }
	int get_Ncols() { return ncols; }

  private:
	int nrows, ncols, maxval;
	std::string header;

	RGB **img;
};

#endif
