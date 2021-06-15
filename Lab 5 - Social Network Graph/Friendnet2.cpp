// Cole Johnston
// Lab 5
// Dr. Gregor
// March 14 2021

#include <set>
#include <vector>
#include <string>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <algorithm>

using namespace std;

void set_oldfriends( vector <string> &name, vector< vector<int> > &friends, int M0, int M1 ) {
	int N = ( int )name.size();
	// initialize 2D-vector-array friends (hint: use vector::assign())
	friends.resize( N );

	for ( int i = 0; i < N; i++ ) {
		set<int> doknow;
		// initialize M (random number of friends: function of M0, M1)
		int M = M0 + rand() % M1;

		while ( ( int )doknow.size() < M ) {
			// compute j (random friend index: hint j != i)
			int j = rand() % N;
			while ( j == i ) { j = rand() % N; }
			doknow.insert(j);
		}

		// use iterator to sweep thru doknow set
		// update the corresponding pairs of friends entries
		for ( set<int>::iterator it = doknow.begin(); it != doknow.end(); it++ ) {
			friends[i].push_back( *it );
			friends[*it].push_back( i );
		}
	}
}

void set_newfriends( vector< vector<int> > &friends, vector< vector<int> > &new_friends ) {
	int N = ( int )friends.size();
	// initialize 2D-vector-array new_friends (hint: use vector::assign())
	new_friends.resize( N );

	for ( int i = 0; i < N; i++ ) {
		// for (each adjacent friend: friends[i][j] == 1) {
		for ( vector<int>::iterator k = friends[i].begin(); k != friends[i].end(); k++ ) {
			// if (k-is-not-i && k-is-not-friend-of-i)
			// update the corresponding pairs of new_friends entries
			vector<int>::iterator f = find( friends[i].begin(), friends[i].end(), *k );
			if ( *k != i && f != friends[i].end() ) {	
				new_friends[i].push_back( *k );
			}
		}
	}

	vector<int>::iterator check;
	for ( int i = 0; i < N; i++ ) {
		sort( new_friends[i].begin(), new_friends[i].end() );
		check = unique( new_friends[i].begin(), new_friends[i].end() );
		new_friends[i].resize( check - new_friends[i].begin() );
	}
}

void writetofile( const char *fname, vector<string> &name, vector< vector<int> > &friends ) {
	// open file
	ofstream out;
	out.open( fname );

	int N = ( int )name.size();
	// determine max name length
	unsigned int max = 0;
	for ( int i = 0; i < N; i++ ) {
		if ( name[i].size() > max )
			max = name[i].size();
	}
	
	for ( int i = 0; i < N; i++ ) {
		// for (each adjacent friend: friends[i][j] == 1) {
		// pretty-print name[i] and name[j] (see assignment) 
		out << setw( max ) << left << name[i] << " :";
		int count, num = 0;
		for ( vector<int>::iterator j = friends[i].begin(); j != friends[i].end(); j++ ) {
			if ( count == 8 ) { out << '\n' << setw( max ) << name[i] << " :"; count = 0; }
			out << ' ' << setw( max ) << name[friends[i][num]];
			++count; ++num;
		}
		out << '\n';
	}

	// close file
	out.close();
}

int main( int argc, char *argv[] ) {
	int seed;
	// parse argc, argv arguments
	// print usage message and exit if invalid
	if ( argc != 1 && argc != 3 ) {
		cerr << "Invalid number of arguments: Expected 1 or 3, given " << argc << '\n';
		return 1;
	}
	else {
		if ( argc == 1 )
			seed = time( NULL );
		else
			seed = atoi( argv[2] );
	}

	// seed random number generator
	srand( seed );

	vector<string> name;

	// read strings from stdin into name vector
	string input;
	while ( cin >> input )
		name.push_back( input );

	int M0 = 1; // min number of friends
	int M1 = 3; // max number of friends

	// declare 2D-vector-array called friends
	// declare 2D-vector-array called new_friends
	vector< vector<int> > friends;
	vector< vector<int> > new_friends;

	set_oldfriends( name, friends, M0, M1 );
	writetofile( "doknow1.txt", name, friends);

	set_newfriends( friends, new_friends );
	writetofile( "mightknow1.txt", name, new_friends );
}
