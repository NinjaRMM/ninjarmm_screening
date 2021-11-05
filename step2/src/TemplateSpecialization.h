#ifndef TEMPLATESPECIALIZATON_H
#define TEMPLATESPECIALIZATON_H

#include <iostream>

template<typename T>    void foo(T a) { std::cout << "1" << std::endl; }
template<>              void foo(int a) { std::cout << "2" << std::endl; }
template<>              void foo(double a) { std::cout << "3" << std::endl; }
template<typename T>    void foo(T* a) { std::cout << "4" << std::endl; }
template<>              void foo(int* a) { std::cout << "5" << std::endl; }
template<>              void foo(double* a) { std::cout << "6" << std::endl; }
                        void foo(int a) { std::cout << "7" << std::endl; }

#endif // TEMPLATESPECIALIZATON_H