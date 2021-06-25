#include <string>
#include <fstream>
#include <iomanip>
#include <vector>
#include <iostream>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <map>
#include <limits>
#include <stdlib.h>
#include <time.h>

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
			if ( j < 0 ) { j = 0; }
			if ( i < j ) {
				int ti = i; int tj = j; 
				i = tj; j = ti;
			} --i;
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
				float ttime = distance / 60.0; //ttime = floor( ttime * 10000 + 0.5 ) / 10000;
				table[1].push_back( ttime ); //cout << i << ":" << j << " -> " << ttime << '\n';
			}
			else { 
				float ttime = distance / 570.0; //ttime = floor( ttime * 10000 + 0.5 ) / 10000;
				table[1].push_back( ttime ); //cout << i << ":" << j << " -> " << ttime << '\n';
			}
		}
	}
}

void read_cityinfo( vector<city> &cities, map<string, int> &dij ) { 
	fstream fin; fin.open( "city_list.txt" );

	string line; int i = 0;

	while ( getline( fin, line ) ) {
		if ( line[0] == '#' || line == "" ) {
			continue;
		}

		city obj;

		istringstream iss ( line ); iss >> obj; 

		cities.push_back( obj );
		dij.insert( make_pair( get_name( cities[i] ), i ) ); ++i;
	}

	fin.close();
}
void write_cityinfo( const vector<city> &cities ) { 
	fstream fout; fout.open( "test.txt" );

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

	int index = 1;
	for ( unsigned int i = 1; i < cities.size(); i++ ) {
		fout << '\n';
		for ( unsigned int j = 0; j < i; j++ ) {
			fout << right << setw(3) << setfill( ' ' ) << index << ' ' << left << setw(38) << setfill('.') 
				<< get_name( cities[i] ) + " to " + get_name( cities[j] ) + ' ';
			fout << right << fixed << setw(7) << setfill(' ') << setprecision(1) << cost( 0, i, j ) << " miles\n";
		}
		++index;
	}
}
void write_timetable( const vector<city> &cities, costtable &cost ) { 
	fstream fout; fout.open( "city_timetable.txt" );

	fout << "TIME TABLE:\n";

	int index = 1;
	for ( unsigned int i = 1; i < cities.size(); i++ ) {
		fout << '\n';
		for ( unsigned int j = 0; j < i; j++ ) {
			fout << right << setw(3) << setfill(' ') << index << ' ' << left << setw(38) << setfill('.') 
				<< get_name( cities[i] ) + " to " + get_name( cities[j] ) + ' ';
			fout << right << fixed << setw(5) << setfill(' ') << setprecision(1) << cost( 1, i, j ) << " hours\n";
		}
		++index;
	}
}

void create_citygraph( const vector <city> &cities, costtable &cost, vector< vector<int> > &graph ) { 
	vector<int> regional, gateway;
	graph.resize( cities.size() );

	// Creating vectors that hold either regional or gateway types
	for ( unsigned int i = 0; i < cities.size(); i++ ) {
		if ( get_type( cities[i] ) == "REGIONAL" ) { regional.push_back( i ); }
		else { gateway.push_back( i ); }
	}
	
	// Linking regional
	for ( unsigned int i = 0; i < regional.size(); i++ ) {
		
		// Linking all regionals within same zone
		for ( unsigned int j = 0; j < regional.size(); j++ ) {
			if ( i == j ) { continue; }
			if ( get_zone( cities[ regional[i] ] ) == get_zone( cities[ regional[j] ] ) ) {
				graph[ regional[i] ].push_back( regional[j] ); graph[ regional[j] ].push_back( regional[i] );
			}
		}

		// Linking closest gateway in same zone
		int count = 0; int min = -1;
		for ( unsigned int j = 0; j < gateway.size(); j++ ) {
			if ( get_zone( cities[ gateway[j] ] ) != get_zone( cities[ regional[i] ] ) ) continue;
			else {
				if ( count == 0 ) { min = gateway[j]; ++count; }
				else {
					if ( cost( 0, regional[i], gateway[j] ) < cost( 0, regional[i], min ) ) 
						min = gateway[j];
				}
			}
		}
		if ( min != -1 ) { graph[ regional[i] ].push_back( min ); graph[ min ].push_back( regional[i] ); }
	}

	// Linking gateway
	vector<int> tmp;
	for ( unsigned int i = 0; i < gateway.size(); i++ ) {
		int zone1 = get_zone( cities[ gateway[i] ] );
		for ( unsigned int j = 0; j < gateway.size(); j++ ) {
			
			// Linking gateways within same zone
			int zone2 = get_zone( cities[ gateway[j] ] );
			if ( i == j ) continue;
			if ( zone1 == zone2 ) {
				graph[ gateway[i] ].push_back( gateway[j] ); graph[ gateway[j] ].push_back( gateway[i] );
				continue;
			}

			// Adding all gateways in other zones to temporary vector
			else if ( cost( 0, gateway[i], gateway[j] ) < 5200 ) {
				tmp.push_back( gateway[j] );
			}
		}
		// Erasing gateways in similar zones that are more milage
		for ( unsigned int x = 0; x < tmp.size(); x++ ) {
			for ( unsigned int y = 0; y < tmp.size(); y++ ) {
				if ( x == y ) continue;
				if ( get_zone( cities[ tmp[x] ] ) == get_zone( cities[ tmp[y] ] ) ) {
					if ( cost( 0, gateway[i], tmp[x] ) < cost( 0, gateway[i], tmp[y] ) )
						tmp.erase( tmp.begin() + y );
					else
						tmp.erase( tmp.begin() + x );
				}
			}
		}
		// Adding remaining gateways to graph
		for ( unsigned int x = 0; x < tmp.size(); x++ ) {
			graph[ gateway[i] ].push_back( tmp[x] ); graph[ tmp[x] ].push_back( gateway[i] );
		} tmp.clear();
	}

	for ( unsigned int i = 0; i < graph.size(); i++ ) {
		sort( graph[i].begin(), graph[i].end() );
		vector<int>::iterator it = unique( graph[i].begin(), graph[i].end() );
		graph[i].resize( distance( graph[i].begin(), it ) );
	}
}
void write_citygraph( const vector <city> &cities, costtable &cost, const vector< vector<int> > &graph ) { 
	fstream fout; fout.open( "city_graph.txt" );
	
	fout << "CITY GRAPH:\n\n";

	for ( unsigned int i = 0; i < graph.size(); i++ ) {
		fout << right << setw(3) << i << ' ' << get_name( cities[i] ) << '\n';
		for ( unsigned int j = 0; j < graph[i].size(); j++ ) {
			fout << right << setw(6) << graph[i][j] << ' ' << left << setw(20) << get_name( cities[ graph[i][j] ] );
			fout << right << setw(6) << fixed << setprecision(1) << cost( 0, i, graph[i][j] ) << " miles";
			fout << right << setw(6) << fixed << setprecision(1) << cost( 1, i, graph[i][j] ) << " hours\n";
		}
		fout << '\n';
	}
}

typedef enum { WHITE, BLACK } vcolor_t;

void dijkstra_route( const vector<city> &V, costtable &W, const vector< vector<int> > &E, int mode, int source, int sink ) { 
	vector<vcolor_t> vcolor; vector<float> vdist; vector<int> vlink;
	
	vcolor.assign( V.size(), WHITE );

	vdist.assign( V.size(), numeric_limits<float>::max() );
	vdist[source] = 0;

	vlink.assign( V.size(), -1 );
	vlink[source] = source;

	while(1) {
		int next_i = -1;
		float mindist = numeric_limits<float>::max();

		for ( int i = 0; i < (int)vcolor.size(); i++ ) {
			if ( ( vcolor[i] == WHITE ) && ( mindist > vdist[i] ) ) {
				next_i = i;
				mindist = vdist[i];
			}
		}

		int i = next_i;
		if ( i == -1 )
			return;

		vcolor[i] = BLACK;

		if ( i == sink )
			break;

		for ( int k = 0; k < (int)E[i].size(); k++ ) {
			int j = E[i][k];
			float wij = W( mode, i, E[i][k] );
			if ( vcolor[j] == WHITE ) {
				if ( vdist[j] > ( vdist[i] + wij ) ) {
					vdist[j] = vdist[i] + wij;
					vlink[j] = i;
				}
			}
		}
	}
	
	sort( vlink.begin(), vlink.end() );
	vector<int>::iterator it = unique( vlink.begin(), vlink.end() );
	vlink.resize( distance( vlink.begin(), it ) );

	float target = vdist[sink]; int curr = sink;
	vector<int> tmp; tmp.push_back( sink );
	while ( target != 0 ) {
		for ( unsigned int i = 0; i < vlink.size(); i++ ) {
			float one = target - vdist[ vlink[i] ];
			float two = W( mode, vlink[i], curr );
			
			if ( vdist[ vlink[i] ] == 0 ) {}
			else if ( mode == 1 ) {
				one = floor( one * 10 + 0.5 ) / 10;
				two = floor( two * 10 + 0.5 ) / 10;
			}
			if ( one == two ) {
				tmp.push_back( vlink[i] ); curr = vlink[i]; target = vdist[ vlink[i] ]; break;
			}
		}
	}
	
	vector<float> other; other.push_back( 0.0 ); float track = 0.0;
	if ( mode == 0 ) {
		for ( unsigned int i = tmp.size() - 1; i > 0; i-- ) {
			track += W( 1, tmp[i], tmp[i-1] );
			other.push_back( track );
		} reverse( other.begin(), other.end() );
		
		for ( unsigned int i = tmp.size() - 1; i >= 0; i-- ) {
			cout << right << fixed << setw(7) << setprecision(1) << vdist[ tmp[i] ] << " miles"
				<< setw(6) << other[i] << " hours :" << right << setw(3) << tmp[i] << ' '
				<< left << setw(17) << get_name( V[ tmp[i] ] );
			
			if ( i == tmp.size() - 1 ) { cout << '\n'; continue; }
			
			cout << right << setw(8) << setprecision(1) << W( 0, tmp[i], tmp[i+1] ) << " miles"
				<< setw(6) << W( 1, tmp[i], tmp[i+1] ) << " hours\n";
			
			if ( i == 0 ) { break; } 
		}
	}
	else {
		for ( unsigned int i = tmp.size() - 1; i > 0; i-- ) {
			track+= W( 0, tmp[i], tmp[i-1] );
			other.push_back( track );
		} reverse( other.begin(), other.end() );
		
		for ( unsigned int i = tmp.size() - 1; i >= 0; i-- ) {
			cout << right << fixed << setw(7) << setprecision(1) << other[i] << " miles"
				<< setw(6) << vdist[ tmp[i] ] << " hours :" << right << setw(3) << tmp[i] << ' '
				<< left << setw(17) << get_name( V[tmp[i] ] );
			
			if ( i == tmp.size() - 1 ) { cout << '\n'; continue; }
			
			cout << right << setw(8) << setprecision(1) << W( 0, tmp[i], tmp[i+1] ) << " miles"
				<< setw(6) << W( 1, tmp[i], tmp[i+1] ) << " hours\n";
			
			if ( i == 0 ) { break; }
		}
	}
}

int main( int argc, char *argv[] ) {
	// commandline option decoding
	if ( argc < 2 || argc > 5 ) {
		cerr << "Invalid commandline: Expected ./Citysim1 -distance|time|graphinfo [-seed=x|y]\n";
		return 1;
	}

	string mode = argv[1];

	// city graph declarations
	map<string, int> dij; vector<city> cities;
	read_cityinfo( cities, dij );
	write_cityinfo( cities );

	// set up costtables
	costtable cost;
	cost.setcosttable( cities );

	vector< vector<int> > graph;
	create_citygraph( cities, cost, graph );

	if ( mode == "-graphinfo" ) {
		write_cityinfo( cities );

		write_distancetable( cities, cost );
		write_timetable( cities, cost );

		write_citygraph( cities, cost, graph );

		// return from program
		return 0;
	}

	if ( mode == "-distance" || mode == "-time" ) {
		string line, from, to, seed1, seed2; istringstream iss; int costmode;
		int num1 = numeric_limits<int>::max(); int num2 = numeric_limits<int>::max();

		if ( mode == "-distance" ) { costmode = 0; } else { costmode = 1; }
	
		srand( time( NULL ) );
		if ( argc == 3 || argc == 4 ) {
			seed1 = argv[2]; seed1.erase( 0, 6 ); num1 = atoi( seed1.c_str() ); srand( num1 );
			if ( argc == 4 ) {
				seed2 = argv[3]; seed2.erase( 0, 6 ); num2 = atoi( seed2.c_str() );
			}
		}

		cout << "Enter> "; int count = 0;
		while ( getline( cin, line ) ) {
			iss.clear(); iss.str( line ); iss >> from >> to;
			if ( from == "*" ) {
				from = get_name( cities[ rand() % cities.size() ] );
			}
			else {
				for ( map<string, int>::iterator i = dij.begin(); i != dij.end(); i++ ) {
					if ( from.compare( 0, from.size(), i -> first, 0, from.size() ) == 0 ) {
						from = i -> first; break;
					}
				}
			}
			if ( to == "*" ) {
				if ( num2 != numeric_limits<int>::max() && count == 0 ) { srand( num2 ); ++count; }
				to = get_name( cities[ rand() % cities.size() ] );
			}
			else {
				for ( map<string, int>::iterator i = dij.begin(); i != dij.end(); i++ ) {	
					if ( to.compare( 0, to.size(), i -> first, 0, to.size() ) == 0 ) {
						to = i -> first; break;
					}
				}
			}

			dijkstra_route( cities, cost, graph, costmode, dij[from], dij[to] );
			
			cout << "\nEnter> ";
		}
	}
}
