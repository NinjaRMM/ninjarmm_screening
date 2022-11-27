#include <iostream>
#include <string>

class A {
    public:
    A() {
        std::cout << "Constructing A\n";
    }
    ~A() {
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
  delete a; //Only the destructor of A is called, althought the constructor of B is called what, may cause memory leak, as shown in the output bellow.
}

/* Output
Constructing A
Constructing B
Constructing A
Constructing B
Destructing B
Destructing A
Destructing A
*/