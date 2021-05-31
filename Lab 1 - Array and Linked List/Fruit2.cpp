// Cole Johnston
// COSC302
// Dr. Gregor

// list all headers needed 
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>

using namespace std;

struct fruit {
	//overload operators <, <=, ==, and +=
	bool operator <( const fruit &obj ) const;
	bool operator <=( const fruit &obj ) const;
	bool operator ==( const fruit &obj ) const;
	int operator +=( fruit &obj ) ;

	//data declarations
	string name;
	float quantity;
	float unicost;
	char check;

};

struct node {
	//constructor
	node();
	//data declarations
	fruit data;
	node *next;
};

bool fruit::operator <( const fruit &obj ) const { return name < obj.name; }

bool fruit::operator <=( const fruit &obj ) const { return name <= obj.name; }

bool fruit::operator ==( const fruit &obj ) const { return name <= obj.name; }

int fruit::operator +=( fruit &obj ) {
	quantity = quantity + obj.quantity;
	return quantity;
}

node::node() {
	next = NULL;
	data = fruit();
}

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

	//declare linked_list<fruit>	

	//open file
	fstream fin;
	string F = argv[2];
	string line;
	istringstream iss;
	fruit f;
	float total = 0.0;
	float price = 0.0;

	fin.open( F.c_str() );

	//while (reading more data)
	//store data INORDER | SORTALL | SORTONE 
	node *head = new node;

	while ( getline( fin, line ) ) {
		iss.clear();
		iss.str( line );

		iss >> f.name >> f.quantity >> f.unicost;

		node *p = head -> next;
		node *prev = head;
		node *pp = new node;
		pp -> data = f;

		if ( mode == "-inorder" ) {
			while ( p != NULL ) {
				if ( p -> next == NULL ) {
					pp -> next = prev -> next;
					prev -> next = pp;
					break;
				}
				p = p -> next;
				prev = prev -> next;
			}
			if ( p == NULL ) {
				pp -> next = prev -> next;
				prev -> next = pp;
			}
		}
		else {
			while ( p != NULL ) {
				if ( ( pp -> data.name ) <= ( p -> data.name ) ) {
					if ( pp -> data.name == p -> data.name && mode == "-sortone" ) {
						p -> data.quantity += pp -> data.quantity;
						break;
					}
					else {
						pp -> next = prev -> next;
						prev -> next = pp;
						break;
					}
				}
				p = p -> next;
				prev = prev -> next;
			}
			if ( p == NULL ) {
				pp -> next = prev -> next;
				prev -> next = pp;
			}
		}
	}

	node *prev = head -> next;
	node *p = prev -> next;
	//pretty-print linked list content to stdout	

	p = head -> next;

	if ( mode == "-inorder" ) {
		while ( p != NULL ) {
			if ( p -> next == NULL ) {
				price = ( p -> data.quantity ) * ( p -> data.unicost );
				total += price;
				cout << fixed << setprecision( 2 ) << left << setw( 20 ) 
					<< setfill( '.' ) << p -> data.name  << setfill( ' ' ) << "  " 
					<< setw( 4 ) << p -> data.quantity << " x " << setw( 4 ) << right 
					<< p -> data.unicost << " = " << setw( 7 ) << price << "  : " 
					<< setw( 8 ) << total << '\n';
			}
			p = p -> next;
		}
	}
	p = head -> next;

	while ( p != NULL ) {
		if ( p -> next == NULL && mode == "-inorder" ) { break; }
		else {
			price = ( p -> data.quantity ) * ( p -> data.unicost );
			total += price;
			cout << fixed << setprecision( 2 ) << left << setw( 20 ) << setfill( '.' )					 
				<< p -> data.name  << setfill( ' ' ) << "  " << setw( 4 )      
				<< p -> data.quantity << " x " << setw( 4 ) << right << p -> data.unicost				 
				<< " = " << setw( 7 ) << price << "  : " << setw( 8 ) << total << '\n';
		}
		p = p -> next;
	}

	//free linked list memory
	while( ( p = head ) != NULL ) {
		head = head -> next;
		delete p;
	}

	return 0;
}
