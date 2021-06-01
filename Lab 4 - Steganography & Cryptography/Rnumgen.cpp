#include <stdlib.h>
#include <algorithm>
#include <numeric>
#include <functional>
#include <vector>
#include "Rnumgen.h"

int rnumgen::rand() const { 
    const double randmax = RAND_MAX + 1.0;
    const double p = ( double )std::rand()/randmax;

    return std::upper_bound( F.begin(), F.end(), p ) - F.begin();
}

rnumgen::rnumgen( int seedvalue, std::vector<int> &v ) {
	srand( seedvalue );

	F.resize( v.size() );
	partial_sum( v.begin(), v.end(), F.begin() );
	std::transform( F.begin(), F.end(), F.begin(), bind2nd( std::divides<float>(), F.back() ) );
}
