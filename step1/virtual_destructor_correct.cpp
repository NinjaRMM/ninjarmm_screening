#include <iostream>
#include <string>

class A {
    public:
    A() {
        std::cout << "Constructing A\n";
    }
    virtual ~A() {
        std::cout << "Destructing A\n";
    }
    
};

class B : public A {
    public:
    B() {
        std::cout << "Constructing B\n";
    }
    ~B() {
        std::cout << "Destructing B\n";
    }
};

int main()
{
  std::string name;
  B* b = new B();
  A* a = new B();
  
  delete b;
  delete a; //The destructor of B and A are called as in the output bellow.
}

/* Output
Constructing A
Constructing B
Constructing A
Constructing B
Destructing B
Destructing A
Destructing B
Destructing A
*/