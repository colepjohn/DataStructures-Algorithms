// Cole Johnston
// Dr. Gregor
// COSC302
// Feb 2 2021

// list all headers needed 
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>

using namespace std;

struct fruit {  
	//overload operator <
	bool operator <( const fruit &obj ) const;
	//data declarations
	string name;
	float quantity;
	float unicost;
	char check;

};

bool fruit::operator <( const fruit &obj ) const { return name < obj.name; } 

int main(int argc, char *argv[])
{
	string mode;
	//prog -inorder|sortall|sortone file.txt
	if ( argc != 3 ) {
		cerr << "Invalid number of arguments";
		return 1;
	}
	//set program mode from command line arg
	else {
		mode = argv[1];
		if ( mode != "-inorder" && mode != "-sortall" && mode != "-sortone" ) {
			cerr << "Invalid command mode";
			return 1;
		}
	}
	//declare array_list<fruit>
	vector<fruit> store;

	//open file
	fstream fin;
	string F = argv[2];
	string line;
	istringstream iss;
	fruit f;
	float total = 0.0;
	float price = 0.0;
	int loop = 0;

	fin.open( F.c_str() );

	//while (reading more data)
	//store data INORDER
	while ( getline( fin, line ) ) {
		iss.clear();
		iss.str( line );

		iss >> f.name >> f.quantity >> f.unicost;
		store.push_back( f );
	}

	//if (mode == SORTALL || mode == SORTONE)
	//apply std::stable_sort
	if ( mode == "-sortall" || mode == "-sortone" ) {
		stable_sort( store.begin(), store.end() );
	}
	//if (mode == SORTONE)
	//combine fruit quanties with same name
	if ( mode == "-sortone" ) {
		while ( loop <= 1 ) {
			for ( unsigned int i = 0; i < store.size(); i++ ) {
				for ( unsigned int a = 0; a < store.size(); a++ ) {
					if ( store[i].name.compare( store[a].name ) == 0 && i != a ) {
						store[i].quantity += store[a].quantity;
						store.erase( store.begin() + a );
					}
				}
			}
			++loop;
		}
	}
	//pretty-print array list content to stdout
	for( unsigned int i = 0; i < store.size(); i++ ) {
		price = store[i].quantity * store[i].unicost;
		total += price;
		cout << fixed << setprecision( 2 ) << left << setw( 20 ) << setfill( '.' ) << store[i].name 
			<< setfill( ' ' ) << "  " << setw( 5 ) << store[i].quantity << "x " << setw( 4 ) 
			<< right << store[i].unicost << " = " << setw( 7 ) << price << "  : " << setw( 8 ) 
			<< total << '\n';
	}
	//free array list memory as necessary

	return 0;
}
