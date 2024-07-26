#ifndef CLASSDEF_HPP_
#define CLASSDEF_HPP_

#include <iostream>

struct X{

    int b;
    int c;
    
    X(): b(0), c(7) { std::cout << "X Ctor" << std::endl; }
   ~X() { std::cout << "X Dtor" << std::endl; }

};

#endif // CLASSDEF_HPP_
