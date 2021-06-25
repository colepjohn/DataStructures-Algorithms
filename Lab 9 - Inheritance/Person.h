#ifndef __PERSON_H__ 
#define __PERSON_H__

// #include WHAT'S NEEDED
#include <string>
#include <fstream>

using namespace std;

enum type_enum { UNKNOWN=-1, FACULTY=0,  ASST_PROF=1, ASSOC_PROF=2, FULL_PROF=3,
                 STUDENT=10, FRESHMAN=11, SOPHOMORE=12, JUNIOR=13, SENIOR=14 };

// class person DEFINITION -- base class
class person {
    public:
        friend ostream & operator << ( ostream &, person & );
        virtual bool operator < ( const person &ptr ) const;
        type_enum get_type();
        type_enum get_category();
    
    protected:
        string name;
        type_enum type;
        type_enum category;
        virtual void print_personinfo ( ostream & ) = 0;
        virtual void print_courseinfo ( ostream & ) = 0;
};

// class faculty DEFINITION -- derived class
class faculty : public person {
    public:
        faculty( string, type_enum, type_enum, const vector<string> & );
        bool operator < ( const faculty & ) const;
        void add_course( const vector<string> & );

    private:
        void print_personinfo( ostream & );
        void print_courseinfo( ostream & );
        vector<string> courselist;
};

// class student DEFINITION -- derived class
class student : public person {
    public:
        student( string, type_enum, type_enum, const vector<string> &, const vector<float> & );
        bool operator < ( const student & ) const;
        void add_course( const vector<string> &, const vector<float> & );

    private:
        void print_personinfo( ostream & );
        void print_courseinfo( ostream & );
        vector<string> courselist;
        vector<float> gp;
};

#endif

