#ifndef SLIST_H
#define SLIST_H

// include header file(s) needed
#include <string>
#include <vector>
#include <algorithm>

// qsort_r3: randomly chosen pivot, 3-way partition {<},{=},{>}

// template <typename T>
template <class T>
class slist {
	private:
		struct node {
			node() { data = T(); next = NULL; }

			// add node(const T &key) { write this }
			node( const T &key ) { data = key; next = NULL; }

			// add overloaded operator< code
			bool operator<( const node &p ) const { return data < p.data; };

			T data;
			node *next;
		};

		// add class sptr { write this for node data }
		class sptr {
			public:    
				sptr( node *_ptr = NULL ) { ptr = _ptr; }    
				bool operator<( const sptr &rhs ) const { return *ptr < *rhs.ptr; }    
				operator node * () const { return ptr; }  
			
			private:    
				node *ptr;
		};

	public:
		class iterator {
			public:
				iterator() { p = NULL; }
				T & operator*() { return p->data; }
				iterator & operator++() { p = p->next; return *this; }
				bool operator!=(const iterator & rhs) const { return p != rhs.p; }

			private:
				iterator(node *n_p) { p = n_p; }
				node *p;

				friend class slist<T>;
		};

	public:
		slist();
		~slist();

		void push_back(const T &);
		void sort(const std::string &);
		void qsort_r3( std::vector<sptr> &, int, int );

		iterator begin() { return iterator(head->next); }
		iterator end() { return iterator(NULL); }

	private:
		node *head;
		node *tail;
};


template <typename T>
slist<T>::slist() {
	head = new node();
	tail = head;
}

template <typename T>
slist<T>::~slist() {
	while (head->next != NULL) {
		node *p = head->next;
		head->next = p->next;
		delete p;
	}
	delete head;

	head = NULL;
	tail = NULL;
}

template <typename T>
void slist<T>::push_back(const T &din) {
	tail->next = new node(din);
	tail = tail->next;
}

template <typename T>
void slist<T>::qsort_r3( std::vector<sptr> &A, int left, int right ) {
	if ( left < right ) {
		srand( time( NULL ) );
		int pvt = rand() % ( right - left + 1 ) + left;

		std::swap( A[right], A[pvt] );
		
		int pindex = left;
		int pivot = right;

		for ( int i = left; i < right; i++ ) {
			if ( A [i] < A[pivot] ) {
				std::swap( A[i], A[pindex] );
				pindex++;
			}
		}

		std::swap( A[pivot], A[pindex] );

		qsort_r3( A, left, pindex - 1 );
		qsort_r3( A, pindex + 1, right );
	}
}

template <typename T>
void slist<T>::sort(const std::string &algname) {
	// determine number of list elements
	node *p = head -> next;
	int count = 0;

	while ( p != NULL ) {
		++count;
		p = p -> next;
	}

	// set up smart pointer array called Ap
	std::vector<sptr> Ap; Ap.resize( count );
	p = head;
	for( int i = 0; i < Ap.size(); i++ ) {
		Ap[i] = p -> next;
		p = p -> next;
	}

	// if quicksort, apply std::sort(...)
	if ( algname == "-quicksort" ) {
		std::sort( Ap.begin(), Ap.end() );
	}
	
	// if mergesort, apply std::stable_sort(...)
	if ( algname == "-mergesort" ) {
		std::stable_sort( Ap.begin(), Ap.end() );
	}

	// if qsort_r3, apply qsort_r3(...)
	if ( algname == "-qsort_r3" ) {
		qsort_r3( Ap, 0, ( Ap.size() - 1 ) );
	}

	// use sorted Ap array to relink list 
	node *link = head;
	for( int i = 0; i < Ap.size(); i++ ) {
		link -> next = Ap[i];
		link = link -> next;
	}
	link -> next = NULL;
}

#endif // SLIST_H
