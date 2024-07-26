#ifndef A_HPP_
#define A_HPP_

#include <iostream>

struct X{
    int b;
    int c;
    
    X(): b(0), c(7) { std::cout << "X Ctor" << std::endl; }
   ~X() { std::cout << "X Dtor" << std::endl; }
};

#endif // A_HPP_
