// #include  WHAT'S NEEDED
#include <vector>
#include <iomanip>
#include <fstream>

using namespace std;

#include "Person.h"

string cole[15] = { "Faculty", "Assistant Professor", "Associate Professor", "Full Professor", 
                    "", "", "", "", "", "", "Student", "Freshman", 
                    "Sophomore", "Junior", "Senior" };

// PERSON CLASS MEMBER FUNCTION IMPLEMENTATION
ostream & operator << ( ostream &out, person &ptr ) {
    ptr.print_personinfo( out ); ptr.print_courseinfo( out );
    return out;
}
bool person::operator < ( const person &ptr ) const { 
    if ( type == ptr.type ) {
        return name < ptr.name;
    } return type < ptr.type;
}
type_enum person::get_type() { return type; }
type_enum person::get_category() { return category; }

// FACULTY CLASS MEMBER FUNCTION IMPLEMENTATION
faculty::faculty( string _name, type_enum _type, type_enum _category, 
                  const vector<string> &courses ) {
    name = _name; type = _type; category = _category; add_course( courses );
}
bool faculty::operator < ( const faculty &ptr ) const { 
    if ( category == ptr.category ) { return name < ptr.name; }
    else { return category < ptr.category; }
}
void faculty::add_course( const vector<string> &cs ) { courselist = cs; }
void faculty::print_personinfo( ostream &out ) {
    out << right << setw(16) << "Name: Dr. " << name << '\n'
        << right << setw(12) << "Category: " << cole[category] << '\n';
}
void faculty::print_courseinfo( ostream &out ) {
    for ( unsigned int i = 0; i < courselist.size(); i++ ) {
        out << right << setw(12) << "Course: " << courselist[i] << '\n';
    } out << '\n';
}

// STUDENT CLASS MEMBER FUNCTION IMPLEMENTATION
student::student( string _name, type_enum _type, type_enum _category,
                  const vector<string> &courses, const vector<float> &gpa  ) {
    name = _name; type = _type; category = _category; add_course( courses, gpa );
}
bool student::operator < ( const student &ptr ) const {
    if ( category == ptr.category ) { return name < ptr.name; }
    else { return category < ptr.category; }
} 
void student::add_course( const vector<string> &cs, const vector<float> &gr ) { 
    courselist = cs; gp = gr; 
}
void student::print_personinfo( ostream &out ) {
    out << right << setw(12) << "Name: " << name << '\n'
        << right << setw(12) << "Category: " << cole[category] << '\n';
}
void student::print_courseinfo( ostream &out ) {
    for ( unsigned int i = 0; i < courselist.size(); i++ ) {
        out << right << setw(12) << "Course: " << left << setw(26) << courselist[i]
            << fixed << setprecision(2) << gp[i] << ' ';
        
        float avg = 0; int count = 0;
        for ( unsigned int j = 0; j <= i; j++ ) {
            avg += gp[j]; ++count;
            if ( j == i ) break;
        } avg /= count; out << fixed << setprecision(2) << avg << '\n';
    } out << '\n';
}
