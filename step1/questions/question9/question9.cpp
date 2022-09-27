// forward example
#include <utility>      // std::forward
#include <iostream>     // std::cout

class String {
private:
	char* name;

public:
	String() {
		this->name = nullptr;
		std::cout << "string initialization"  << std::endl;
	}

	String(const char* name) {
		this->name = new char[strlen(name) + 1];
		strcpy(this->name, name);
		std::cout << "string initialization :" << name << std::endl;

	}

	// copy constructor
	String(const String& other) {
		this->name = new char[strlen(other.name) + 1];
		strcpy(this->name, other.name);
		std::cout << "string copy constructor :" << name << std::endl;

	}

	// copy assignment
	String& operator=(const String& rhs) {

		// self-asignment check
		if (this != &rhs) {


			// delete old data
			if (this->name) {
				delete[] this->name;
			}

			this->name = new char[strlen(rhs.name) + 1];
			strcpy(this->name, rhs.name);
			std::cout << "string copy assignment :" << name << std::endl;

		}

		return *this;
	}
	// move constructor
	String(String&& other) {
		this->name = other.name;
		other.name = nullptr;
		std::cout << "string move constructor :" << name << std::endl;

	}

	// move assignment
	String& operator=(String&& rhs) {
		// self-asignment check
		if (this != &rhs) {


			// delete old data
			if (this->name) {
				delete[] this->name;
			}

			this->name = rhs.name;
			rhs.name = nullptr;
			std::cout << "string move assignment :" << name << std::endl;

		}

		return *this;
	}
	const char* GetName() const
	{
		return this->name;
	}
	~String() {
		delete[] this->name;
	}
};


void overloaded(const String &x) { std::cout << " [lvalue]= "<< x.GetName(); }


void overloaded(String&& x) { std::cout << " [rvalue]=" << x.GetName(); }

// function template taking rvalue reference to deduced type
template <class T> void testMethod(T&& x) {
	overloaded(x);                   // always an lvalue
	overloaded(std::forward<T>(x));  // rvalue if argument is rvalue
}

//real usages

template <typename T>
void fooString(T&& arg)
{
	String var = std::forward<T>(arg);
	// Noncompliant : std::move applied to a forwarding reference
	//String var = std::move(arg); 

}


template <class T1, typename T2>
void real_func(T1&& t1, T2&& t2) {
	T1  t1Temp = std::forward<T1>(t1);
	T2   t2Temp = std::forward<T2>(t2);

}

template <typename T1, typename T2>
void wrapper(T1 && e1, T2 && e2) {
	real_func(std::forward<T1>(e1), std::forward<T2>( e2));
}

int main() {
	String tempToMove("move data");
	String temp2("copy data");
	wrapper(std::move(tempToMove), temp2);                 
	
	std::cout << "rvalue move example 1" << std::endl;
	String tempValue("ola");
	fooString(std::move(tempValue));
	std::cout << std::endl << "rvalue move example 1" << std::endl;
	fooString(String("ola 2"));

	std::cout << std::endl<< "lvalue copy example" << std::endl;
	String tempValue2("ola");
	fooString(tempValue2);

	std::cout << std::endl;


	std::cout << "calling  with lvalue: " << std::endl;
	String a("luis");
	testMethod(a);
	std::cout << std::endl;

	std::cout << "calling fn with rvalue: " << std::endl;
	testMethod(String("Pepe"));
	std::cout << std::endl;

	return 0;
}