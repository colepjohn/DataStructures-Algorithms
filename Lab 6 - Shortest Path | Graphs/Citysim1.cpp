#include <string>
#include <fstream>
#include <iomanip>
#include <vector>
#include <iostream>
#include <sstream>
#include <cmath>

using namespace std;

class city {
	public:
		friend int get_zone( const city &obj ) { return obj.zone; }
		friend int get_population( const city &obj ) { return obj.population; }
		friend string get_name( const city &obj ) { return obj.name; }
		friend string get_type( const city &obj ) { return obj.type; }
		friend float get_latitude( const city &obj ) { return obj.latitude; }
		friend float get_longitude( const city &obj ) { return obj.longitude; }

		friend istream & operator >> ( istream &, city & );
		friend ostream & operator << ( ostream &, const city );

	private:
		string name, type;
		int zone, population;
		float latitude, longitude;
};

istream & operator >> ( istream &in, city &obj ) { 
	in >> obj.zone >> obj.name >> obj.type >> obj.latitude >> obj.longitude 
	   >> obj.population;
	return in;
}
ostream & operator << ( ostream &out, const city obj ) { 
	out << left << setw(18) << obj.name << setw(12) << obj.type << setw(10)
		<< obj.population << setw(2) << obj.zone << setw(8) 
		<< fixed << setprecision(2) << obj.latitude << obj.longitude;
	return out;
}

class costtable {
	public:
		void setcosttable( const vector<city> & );
		
		void write_distancetable( vector<city> &, costtable & );
		void write_timetable( vector<city> &, costtable & );

		vector<float> table[2];

		float operator () ( int mode, int i, int j ) {
			if ( i <= j ) {
				int ti = i; int tj = j; 
				i = tj; j = ti;
			}
			return table[ mode ][ i * ( i + 1 ) / 2 + j ];
		}
};

void costtable::setcosttable( const vector<city> &cities ) {
	for ( unsigned int i = 1; i < cities.size(); i++ ) {
		float lat1 = ( get_latitude( cities[i] ) * M_PI / 180 );
		float lon1 = ( get_longitude( cities[i] ) * M_PI / 180 );
		
		string type1 = get_type( cities[i] );
		
		for ( unsigned int j = 0; j < i; j++ ) {
			float lat2 = ( get_latitude( cities[j] ) * M_PI / 180.0 ); float dlat = ( lat1 - lat2 );
			float lon2 = ( get_longitude( cities[j] ) * M_PI / 180.0 ); float dlon = ( lon1 - lon2 );

			float tmp = asin( sqrt( sin( dlat / 2.0 ) * sin( dlat / 2.0 ) + cos( lat1 ) * cos( lat2 ) * sin( dlon / 2.0 ) * sin( dlon / 2.0 ) ) );
			float distance = 2.0 * tmp * 3982.0; distance = 10.0 * round( distance / 10.0 );

			table[0].push_back( distance );

			string type2 = get_type( cities[j] );

			if ( type1 == "REGIONAL" && type2 == "REGIONAL" ) {
				float ttime = distance / 60.0;
				table[1].push_back( ttime );
			}
			else { 
				float ttime = distance / 570.0;
				table[1].push_back( ttime );
			}
		}
	}
}

void read_cityinfo( vector<city> &cities ) { 
	fstream fin; fin.open( "city_list.txt" );
		
	string line;
	
	while ( getline( fin, line ) ) {
		if ( line[0] == '#' || line == "" ) {
			continue;
		}
		
		city obj;

		istringstream iss ( line ); iss >> obj; 
		
		cities.push_back( obj );
	}
	
	fin.close();
}
void write_cityinfo( const vector<city> &cities ) { 
	fstream fout; fout.open( "city_info.txt" );
	
	if ( fout.is_open() ) {	
		for ( unsigned int i = 0; i < cities.size(); i++ ) {
			fout << cities[i] << '\n';
		}
	}
	
	fout.close();
}

void write_distancetable( const vector<city> &cities, costtable &cost ) { 
	fstream fout; fout.open( "city_distancetable.txt" );
	
	fout << "DISTANCE TABLE:\n";

	for ( unsigned int i = 1; i < cities.size(); i++ ) {
		fout << '\n';
		for ( unsigned int j = 0; j < i; j++ ) {
			fout << setw(3) << setfill(' ') << right << i << ' ' << left << setfill('.') << setw(38) 
				<< get_name( cities[i] ) + " to " + get_name( cities[j] ) + ' ';
			fout << setfill(' ') << setw(7) << right << fixed << setprecision(1) << cost( 0, ( i - 1 ), j ) << " miles\n";
		}
	}
}
void write_timetable( const vector<city> &cities, costtable &cost ) { 
	fstream fout; fout.open( "city_timetable.txt" );

	fout << "TIME TABLE:\n";

	int index = 1;
	for ( unsigned int i = 1; i < cities.size(); i++ ) {
		fout << '\n';
		for ( unsigned int j = 0; j < i; j++ ) {
			fout << setw(3) << setfill(' ') << right << i << ' ' << left << setfill('.') << setw(38) 
				<< get_name( cities[i] ) + " to " + get_name( cities[j] );
			fout << setfill(' ') << setw(3) << right << fixed << setprecision(1) << cost( 1, ( i - 1 ), j ) << " hours\n";
		}
		++index;
	}
}

void create_citygraph() { 

}
void write_citygraph() { 

}

void dijkstra_route() { 

}

int main( int argc, char *argv[] ) {
	// commandline option decoding
	if ( argc > 2 ) {
		cerr << "Invalid commandline: Expected ./Citysim1 [-graphinfo]\n";
		return 1;
	}
	
	string mode;
	if ( argc == 2 ) { mode = argv[1]; }

	// city graph declarations

	vector<city> cities;
	read_cityinfo( cities );
	write_cityinfo( cities );
	
	// set up costtables
	costtable cost;
	cost.setcosttable( cities );

	// create_citygraph()

	if ( mode == "-graphinfo" ) {
		write_cityinfo( cities );

		write_distancetable( cities, cost );
		write_timetable( cities, cost );

		write_citygraph();

		// return from program
		return 0;
	}

	// while (not done) {
		// enter> from, to cities
		// dijkstra_route(costmode,from,to) 
	// }
}
