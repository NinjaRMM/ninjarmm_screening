#include <cstdio>

// Global static variable or function in a c/cpp file

// This variable is visible only inside this file
static int privateVar = 123;

// This function is visible only inside this file
static int min(int a, int b) { return (a < b) ? a : b; }

// Static member function of a class/struct:
struct Foo {
    static size_t counter() {
        return count++;
    }

    // Static variable of a class/struct:
    inline static size_t count = 0; // [C++17] inline static
};

// Local static variable inside a function
size_t counter() {
    static size_t count = 0;
    return count++;
}

int
main() {
    printf("%zu\n", counter());
    printf("%zu\n", counter());
    printf("%zu\n", counter());

    printf("%zu\n", Foo::counter());
    printf("%zu\n", Foo::counter());
    printf("%zu\n", Foo::counter());

    return 0;
}