#include <string>
#include <sstream>
#include <iostream> 

// function with lvalue and rvalue reference overloads:
void overloaded (const int& x) ;
void overloaded (int&& x) ;

// function template taking rvalue reference to deduced type:
template <class T> void fn (T&& x) {
	overloaded (x);                   // always an lvalue
	overloaded (std::forward<T>(x));  // rvalue if argument is rvalue
	}

int main () {
	int a;

	std::cout << "calling fn with lvalue: ";
	fn (a);
	std::cout << '\n';

	std::cout << "calling fn with rvalue: ";
	fn (0);
	std::cout << '\n';

	return 0;
