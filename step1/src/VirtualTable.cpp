#include <iostream>

// A quick example with a trick that I dicovered recently. Although it is not possible to 
// change the function pointers in a vtable, it is still possible to change the vtable address
// to which an object is pointing to, and this address is at the begining of an object.

class NonSense {
public:
  virtual void saySomething() {
    std::cout << "Something to be said.\n";
  }
};

class Foo : public NonSense {
public:
  void saySomething() override {
    std::cout << "Foo.\n";
  }
};

class Bar : public NonSense {
public:
  void saySomething() override {
    std::cout << "Bar.\n";
  }
};

int main() {
  Foo* foo = new Foo();
  Bar* bar = new Bar();

  std::cout << "Before switching:" << "\n";
  foo->saySomething();
  bar->saySomething();

  unsigned long long* v_table_foo = (unsigned long long*)foo;
  unsigned long long* v_table_bar = (unsigned long long*)bar;
  unsigned long long v_table_temp = *v_table_bar;

  *v_table_bar = *v_table_foo;
  *v_table_foo = v_table_temp;

  std::cout << "\nAfter switching:" << "\n";
  foo->saySomething();
  bar->saySomething();

  return 0;
}
