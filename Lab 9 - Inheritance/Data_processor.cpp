// #include WHAT'S NEEDED
#include <vector>
#include <string>
#include <iostream>
#include <stdlib.h>

using namespace std;

#include "Person.h"
#include "Sptrsort.h"

int main( int argc, char *argv[] ) {
	if ( argc != 2 ) {
	  cerr << "usage: ./sdata_processor -person|faculty|student < file.xml\n";
	  return 0;
	}

	string mode = argv[1]; bool per = false, fac = false, stu = false;
	if ( mode == "-person" ) { per = true; }
	else if ( mode == "-faculty" ) { fac = true; }
	else if ( mode == "-student" ) { stu = true; }

	person *n_person; student *n_student; faculty *n_faculty;
	vector<person *> person_list; vector<student *> student_list; vector<faculty *> faculty_list;

	type_enum type = UNKNOWN;
	type_enum g = UNKNOWN;

	int line = 1;
	size_t iL, iR;
	string input;
	string name, course, cat, num, grade; float gpa = 0;
	vector<string> courses; vector<float> gp;

	// CS307: CHECK CORRECTESS OF XML HEADER 

	while ( getline( cin, input ) ) {
		++line;

		if ( ( iL = input.find('<') ) == string::npos ) {
			continue;

		} else if ( input.compare( iL, 9, "<faculty>" ) == 0 ) {
			// set type
			type = FACULTY;
			courses.clear();

		} else if ( input.compare( iL, 10, "</faculty>" ) == 0 ) {

			// CS307: CHECK CORRECTESS OF XML DATA
			// CODE FOR ADDING FACULTY PERSON TO DATABASE

			// check type
			// set subtype
			if ( type == FACULTY ) {
				if ( cat == "Assistant Professor" ) { g = ASST_PROF; } 
				else if ( cat == "Associate Professor" ) { g = ASSOC_PROF; } 
				else if ( cat == "Full Professor" ) { g = FULL_PROF; }
			}

			// sort( courses.begin(), courses.end() );
			// n_person = new faculty( name, type, g, courses );
			// person_list.push_back( n_person );

			if ( per == true || fac == true ) {
				if ( per == true ) {	
					n_person = new faculty( name, type, g, courses );
					person_list.push_back( n_person );
				} else {
					n_faculty = new faculty( name, type, g, courses );
					faculty_list.push_back( n_faculty );
				}
			}

			// reset type
			// reset subtype
			type = UNKNOWN; g = UNKNOWN; name.clear();

			continue;

		} else if ( input.compare( iL, 9, "<student>" ) == 0 ) {
			// set type
			type = STUDENT;
			courses.clear();
			gp.clear();

		} else if ( input.compare( iL, 10, "</student>" ) == 0 ) {

			// CS307: CHECK CORRECTESS OF XML DATA
			// CODE FOR ADDING STUDENT PERSON TO DATABASE

			// check type 
			// set subtype
			if ( type == STUDENT ) {
				if ( cat == "Freshman" ) { g = FRESHMAN; } 
				else if ( cat == "Sophomore" ) { g = SOPHOMORE; } 
				else if ( cat == "Junior" ) { g = JUNIOR; } 
				else if ( cat == "Senior" ) { g = SENIOR; }
			}

			// sort( courses.begin(), courses.end() );
			// n_person = new student( name, type, g, courses, gp );
			// person_list.push_back( n_person );

			if ( per == true || stu == true ) {
				if ( per == true ) {
					n_person = new student( name, type, g, courses, gp );
					person_list.push_back( n_person );
				} else {
					n_student = new student( name, type, g, courses, gp );
					student_list.push_back( n_student );
				}
			}

			// reset type
			// reset subtype
			type = UNKNOWN; g = UNKNOWN;

			continue;

		} else if ( input.compare( iL, 5, "<name" ) == 0 ) {
			iL = input.find( "=\"", iL );
			iR = input.find( "\"/>", iL + 2 );

			// CS307: CHECK CORRECTESS OF XML DATA
			// CODE FOR EXTRACTING NAME (using iL, iR)
			name = input; name.erase( 0, 9 ); name.erase( iR - 9 , 3 );

		} else if ( input.compare( iL, 10, "<category=" ) == 0 ) {
			iL = input.find( "=\"", iL );
			iR = input.find( "\"/>", iL + 2 );

			// CS307: CHECK CORRECTESS OF XML DATA
			// CODE FOR EXTRACTING CATEGORY (using iL, iR)
			cat = input; cat.erase( 0, 13 ); cat.erase( iR - 13, 3 );

		} else if ( input.compare( iL, 7, "<course" ) == 0 ) {
			iL = input.find( "=\"", iL );
			iR = input.find( "\"", iL + 2 );

			// CS307: CHECK CORRECTESS OF XML DATA
			// CODE FOR EXTRACTING COURSE (using iL, iR)
			course = input; course.erase( 0, 11 ); course.erase( iR - 11, course.size() - iR - 11 );
			unsigned int i = 0;
			if ( !( courses.empty() ) ) {
				for ( i = 0; i < courses.size(); i++ ) {
					if ( course < courses[i] ) { 
						courses.insert( courses.begin() + i, course ); 
						break; 
					}
					else if ( ( i + 1 ) == courses.size() ) { 
						courses.push_back( course ); 
						break; 
					}
				}
			}
			else courses.push_back( course );

			iL = iR;
			if ( type == FACULTY ) {
				iR = input.find( "/>", iL + 1 );
			} else if ( type == STUDENT ) {
				iL = input.find( "gp=\"", iL );
				iR = input.find( "\"/>", iL + 4 );

				// CS307: CHECK CORRECTESS OF XML DATA
				// CODE FOR EXTRACTING GRADE (using iL, iR)
				grade = input; grade.erase( 0, iL + 4 ); grade.erase( grade.size() - 3, 3 );
				gpa = atoi( grade.c_str() ); if ( grade.size() > 1 ) { gpa += 0.5; }
				if ( gp.empty() ) gp.push_back( gpa );
				else gp.insert( gp.begin() + i, gpa );
			}
		}
	}

	// EXTRACT LIST OF POINTERS TO PERSON, FACULTY OR STUDENT
	// ENTRY, SORT USING SPTRSORT AND PRINT RESULT TO STDOUT
	if ( per == true ) {
		sptrsort( person_list );
		for ( unsigned int i = 0; i < person_list.size(); i++ ) {
			cout << *person_list[i];
		}
	} else if ( fac == true ) {
		sptrsort( faculty_list );
		for ( unsigned int i = 0; i < faculty_list.size(); i++ ) {
			cout << *faculty_list[i];
		}
	} else if ( stu == true ) {
		sptrsort( student_list );
		for ( unsigned int i = 0; i < student_list.size(); i++ ) {
			cout << *student_list[i];
		}
	}

	// NOTE: PERSON, FACULTY AND STUDENT EACH HAVE THEIR OWN
	// OVERLOADED COMPARISON OPERATOR (--> DIFFERENT SORTING)

	// RELEASE ALL DYNAMICALLY ALLOCATED MEMORY
}
