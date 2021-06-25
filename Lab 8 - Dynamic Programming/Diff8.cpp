#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <stack>

using namespace std;

template <typename T>
class matrix {
	// class for storing 2D array
	public:
		matrix() { data = NULL; }
		
		~matrix() {
			if ( data ) {
				if ( data[0] )
					delete [] data[0];
				delete [] data;

				data = NULL;
			}
		}

		void assign( int _Nrows, int _Ncols, const T &value = T() ) {
			Nrows = _Nrows;
			Ncols = _Ncols;

			data = new T *[Nrows];
			data[0] = new T [Nrows * Ncols];
			for ( int i = 1; i < Nrows; i++ )
				data[i] = data[i - 1] + Ncols;

			for ( int i = 0; i < Nrows; i++ ) {
				for ( int j = 0; j < Ncols; j++ )
					data[i][j] = value;
			}
		}

		int get_Nrows() const { return Nrows; }
		int get_Ncols() const { return Ncols; }

		T * operator[] ( int i ) { return data[i]; }

	private:
		int Nrows, Ncols;
		T **data;
};

class LCS {
	public:
		LCS();
		void text1_push_back( string &line ) { text1.push_back( line ); }
		void text2_push_back( string &line ) { text2.push_back( line ); }
		void compute_alignment();
		void report_difference();

	private:
		// support functions
		void report_difference( int, int, int, vector<string> &, vector<string> &, vector<string> &, vector<int> & );
		void condense( const vector<string> &, const vector<int> &, int );

		int ( LCS::*DEL )( string );
		int ( LCS::*INS )( string );
		int ( LCS::*SUB )( string, string );

		int DELcost( string s ) { return ( *this.*DEL )( s ); }    
		int INScost( string s ) { return ( *this.*INS )( s ); }    
		int SUBcost( string s1, string s2 ) { return ( *this.*SUB )( s1, s2 );}

		int DEL1( string s ) { return 1; }    
		int INS1( string s ) { return 1; }    
		int SUB1( string s1, string s2 ) { return s1 == s2 ? 0 : m + n; }

		matrix<int> cost;
		matrix<int> link;

		int m, n;
		string x, y;
		
		const int VERT;
		const int HORZ;
		const int DIAG;

		// text1 and text2 buffer
		vector<string> text1;
		vector<string> text2;
};

LCS::LCS() : VERT(1), HORZ(2), DIAG(4) {
	DEL = &LCS::DEL1;
	INS = &LCS::INS1;
	SUB = &LCS::SUB1;
}

void LCS::compute_alignment() {
	m = text1.size() - 1; n = text2.size() - 1;

	cost.assign( m + 1, n + 1 );
	link.assign( m + 1, n + 1 );

	cost[0][0] = 0; link[0][0] = 0;

	for ( int i = 1; i <= m; i++ ) {      
		cost[i][0] = cost[i-1][0] + DELcost( text1[i] );
		link[i][0] = VERT;
	}
	
	for ( int j = 1; j <= n; j++ ) {      
		cost[0][j] = cost[0][j-1] + INScost( text2[j] );      
		link[0][j] = HORZ;    
	}    
	
	for ( int i = 1; i <= m; i++ ) {      
		for (int j = 1; j <= n; j++ ) {        
			cost[i][j] = cost[i-1][j-1] + SUBcost( text1[i], text2[j] );        
			link[i][j] = DIAG;
			
			int delcost = cost[i-1][j] + DELcost( text1[i] );        
			if ( delcost < cost[i][j] ) {          
				cost[i][j] = delcost;          
				link[i][j] = VERT;        
			}        
			
			int inscost = cost[i][j-1] + INScost( text2[j] );        
			if ( inscost < cost[i][j] ) {          
				cost[i][j] = inscost;          
				link[i][j] = HORZ;        
			}      
		}    
	}
}

void LCS::report_difference() { 
	vector<string> ax; vector<string> ay; vector<string> edits; vector<int> num;
	
	report_difference( m, n, link[m][n], ax, ay, edits, num ); 
}
// num = ( x * ( max x ) ) + y
// x = num / ( max x )
// y = num % ( max x )
void LCS::report_difference( int i, int j, int ij, vector<string> &ax, vector<string> &ay, vector<string> &edits, vector<int> &num ) {
	if ( ij != 0 ) {
		if ( ij == DIAG ) {
			ax.push_back( text1[i] );
			ay.push_back( text2[j] );
			edits.push_back( "MATCH" ); num.push_back( ( i * m ) + j );
			i -= 1; j -= 1;
		} else if ( ij == VERT ) {
			ax.push_back( text1[i] );
			ay.push_back( text2[0] );
			edits.push_back( "DEL" ); num.push_back( ( i * m ) + j ); 
			i -= 1;
		} else { // ij == HORZ
			ax.push_back( text1[0] );
			ay.push_back( text2[j] );
			edits.push_back( "INS" ); num.push_back( ( i * m ) + j );
			j -= 1;
		}
		ij = link[i][j]; report_difference( i, j, ij, ax, ay, edits, num );
	}
	else { int i = edits.size() - 1; condense( edits, num, i ); }
}

void LCS::condense( const vector<string> &edits, const vector<int> &num, int i ) {
	int count = 0;
	
	if ( edits[i] == "MATCH" ) { if ( i != 0 ) { condense( edits, num, --i ); } else { return; } }
	
	else {
		while ( edits[i] != "MATCH" ) { ++count; if ( i != 0 ) { --i; } else { break; } } if ( i != 0 ) { ++i; }
		
		if ( count == 1 ) {
			int x = num[i] / m; int y = num[i] % m; cout << x;
			if ( edits[i] == "INS" ) { cout << 'a'; } else { cout << 'd'; } cout << y << '\n';
			
			if ( edits[i] == "INS" ) { cout << "> " << text2[y] << '\n'; }
			else { cout << "< " << text1[x] << '\n'; }

			if ( i != 0 ) { condense( edits, num, --i ); } else { return; }
		}
		
		if ( count == 2 ) {
			if ( edits[i] == edits[i+1] ) {
				if ( edits[i] == "DEL" ) { 
					int y = num[i] / m; int x = num[i+1] / m; int f = num[i] % m;
					cout << x << ',' << y << 'd' << f << "\n< " << text1[x] << "\n< " << text1[y] << '\n';
				}
				else {
					int y = num[i] % m; int x = num[i+1] % m; int f = num[i] / m;
					cout << f << 'a' << x << ',' << y << "\n> " << text2[x] << "\n> " << text2[y] << '\n';
				}
			} else { 
				int x = num[i+1] % m; int y = num[i] / m;
				cout << x << 'c' << y << "\n< " << text1[x] << "\n---\n> " << text2[y] << '\n';
			} if ( i != 0 ) { condense( edits, num, --i ); } else { return; }
		}
		
		else if ( count > 2 )  { 
			int x1, y1, x2, y2;
			for ( int c = 0; c < 2; c++ ) {
				if ( c == 0 ) {
					x1 = num[i] / m; y1 = num[i + count/2-1] / m;
					cout << y1 << ',' << x1 << 'c';
				} else {
					y2 = num[i+count/2] % m; x2 = num[i+count-1] % m;
					cout << x2 << ',' << y2 << '\n';
				}
			}
			
			for ( int t = y1; t <= x1; t++ ) { 
				cout << "< " << text1[t] << '\n';
				if ( t == x1 ) break;
			} cout << "---\n";
			for ( int t = x2; t <= y2; t++ ) { 
				cout << "> " << text2[t] << '\n'; 
				if ( t == y2 ) break;
			}
			
			if ( i != 0 ) { condense( edits, num, --i ); } else { return; }
		}
	}
}

int main(int argc, char *argv[]) {
	// check two input files are specified on command line
	if ( argc < 2 || argc > 3 ) { cerr << "usage: ./Diff8 file1 file2"; return 1; }

	LCS lcs;  // instantiate your "LCS based diff" object

	// read the text from file1 into the lcs.text1 buffer
	fstream file1; file1.open( argv[1] ); string line; string s = ""; lcs.text1_push_back( s );
	while ( getline( file1, line ) ) {
		lcs.text1_push_back( line );
		line.clear();
	} 

	// read the text from file2 into the lcs.text2 buffer
	fstream file2; file2.open( argv[2] ); line.clear(); lcs.text2_push_back( s );
	while ( getline( file2, line ) ) {
		lcs.text2_push_back( line );
		line.clear();
	}

	lcs.compute_alignment();
	lcs.report_difference();
}
