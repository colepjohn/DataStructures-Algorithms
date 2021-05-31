// Cole Johnston
// COSC302
// Lab 2

// include header files needed
#include <fstream>
#include <string>
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <vector>
#include <stdlib.h>
#include <time.h>

using namespace std;

class data {
	public:
		// add operator< using lastname, firstname, phone number
		bool operator <( const data &obj ) const;

		friend istream & operator>>(istream &, data &);
		friend ostream & operator<<(ostream &, const data &);

	private:
		string firstname;
		string lastname;
		string phonenum;
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

istream & operator>>(istream &in, data &r) { 
	// write this to read data object data
	in >> r.firstname >> r.lastname >> r.phonenum;
	return in;
}

ostream & operator<<(ostream &out, const data &r) {
	// write this to write data object data
	out << left << setw(24) << r.lastname + ", " + r.firstname 
		<< r.phonenum << '\n';
	return out;
}

template <typename Tdata>
void quicksort( vector<Tdata> &A, int left, int right) { 
	// write this
	if ( left < right ) {
		//int pindex = partition( A, left, right );
		srand( time( NULL ) );
		int pvt = rand() % ( right - left + 1 ) + left;
		
		swap( A[right], A[pvt] );

		int pindex = left;
		int pivot = right;

		for ( int i = left; i < right; i++ ) {
			if ( A[i] < A[pivot] ) {
				swap( A[i], A[pindex] );
				pindex++;
			}
		}

		swap( A[pivot], A[pindex] );

		quicksort( A, left, pindex - 1 );
		quicksort( A, pindex + 1, right );
	}
}

template <typename Tdata>  
void quickselect( vector<Tdata> &A, int left, int k, int right ) { 
// write this 
	while ( 1 ) {
		//int pindex = partition( A, left, right );
		srand( time ( NULL ) );
		int pvt = rand() % ( right - left + 1 ) + left;

		swap( A[right], A[pvt] );
		
		int pindex = left;
		int pivot = right;

		for ( int i = left; i < right; i++ ) {
			if ( A[i] < A[pivot] ) {
				swap( A[i], A[pindex] );
				pindex++;
			}
		}

		swap( A[pivot], A[pindex] );

		if ( pindex == k )
			return;

		if ( k < pindex ) right = pindex - 1;
		else			  left = pindex + 1;
	}
}

void printlist( vector<data>::iterator begin, vector<data>::iterator end ) { 
	// see Lab 1
	while ( begin != end ) {
		cout << *begin;
		begin++;
	}
}

int main(int argc, char *argv[]) {
	// perform command-line error checking
	string mode;
	if ( argc == 3 || argc == 5 ) {
		mode = argv[1];
		if ( mode != "-stl" && mode != "-rpivot" ) {
			cerr << "Invalid command\n";
			return 1;
		}
	}
	else {
		cerr << "Invalid number of arguments\n";
		return 1;
	}
	// usage: Qsort -stl | -rpivot [k0 k1] file.txt

	// open file.txt
	fstream fin;
	string F;
	if ( argc == 5 ) { F = argv[4]; }
	else { F = argv[2]; }
	
	fin.open( F.c_str() );

	vector<data> A;

	data din;
	while (fin >> din)
		A.push_back(din);

	// close file.txt
	fin.close();

	if ( mode == "-stl" ) {
		sort( A.begin(), A.end() );
	} 
	else if ( mode == "-rpivot" ) {
		int N = ( int )A.size();
		int k0 = 0;
		int k1 = N - 1;

		if ( argc == 5 ) {
			k0 = atoi( argv[2] );
			k1 = atoi( argv[3] );
		}

		// if specified, update k0, k1 and apply quickselect 
		// to ensure that A[0:k0-1] <= A[k0:k1] <= A[k1+1:N-1]
		if ( k0 < 0 ) { k0 = 0; }
		if ( k1 >= N ) { k1 = N - 1; }

		quickselect( A, 0, k0, N - 1 );
		quickselect( A, k0, k1, N - 1 );
		
		quicksort(A, k0, k1);
	} 

	printlist(A.begin(), A.end());
}
