// include header file(s) needed
#include "Slist.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

using namespace std;

// modify the data class and the associated input 
// and output operators to store, set and print a 
// line number identifier called ID

/*
   class data { copy from QsortA.cpp };
   istream & operator>> (...) { copy from Qsort.cpp }
   ostream & operator<< (...) { copy from Qsort.cpp }
   */
class data {
	public:
		bool operator <( const data &obj ) const;

		friend istream & operator>>( istream &, data & );
		friend ostream & operator<<( ostream &, const data & );

	private:
		string firstname;
		string lastname;
		string phonenum;
		int ID = 0;
};

bool data::operator <( const data &obj ) const {
	if ( lastname == obj.lastname ) {
		if ( firstname == obj.firstname ) {
			return phonenum < obj.phonenum;
		}
		else {
			return firstname < obj.firstname;
		}
	}
	else {
		return lastname < obj.lastname;
	}
}

istream & operator>>( istream &in, data &r ) {
	// write this to read data object data
	in >> r.firstname >> r.lastname >> r.phonenum;
	++r.ID;
	return in;
}

ostream & operator<<( ostream &out, const data &r ) {
	// write this to write data object data
	out << left << setw(24) << ' ' + r.lastname + ' ' + r.firstname
		<< ' ' + r.phonenum << setw(9) << right << r.ID << '\n';
	return out;
}

//void printlist(...) { copy from QsortA.cpp }
void printlist( slist<data>::iterator begin, slist<data>::iterator end ) {
	while ( begin != end ) {	
		cout << *begin;
		++begin;
	}
}

int main( int argc, char *argv[] ) {
	// copy command-line check from Qsort.cpp,
	// then modify to apply to this assignment
	if ( argc != 3 ) {
		cerr << "Invalid number of arguments\n";
		return 1;
	}

	string mode;
	mode = argv[1];
	if ( mode != "-mergesort" && mode != "-quicksort" && mode != "-qsort_r3" ) {
		cerr << "Invalid sort mode";
		return 1;
	}

	// open input file
	fstream fin;
	string F = argv[2];
	fin.open( F.c_str() );

	slist<data> A;

	data din;
	while ( fin >> din )
		A.push_back( din );

	// close input file
	fin.close();

	//A.sort(command line mode option);
	A.sort( mode );

	printlist( A.begin(), A.end() );
}
