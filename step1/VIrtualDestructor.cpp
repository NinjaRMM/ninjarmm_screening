	
#include <string>
#include <sstream>
#include <iostream> 

class base {
public:
	base()    
		{ std::cout << "Constructing base\n"; }
	virtual ~base()
		{ std::cout << "Destructing base\n"; }    
	};

class derived : public base {
public:
	derived()    
		{ std::cout << "Constructing derived\n"; }
	virtual ~derived()
		{ std::cout << "Destructing derived\n"; }
	};

int main()
	{
	derived *d = new derived(); 
	base *b = d;
	delete b;
	return 0;
	}