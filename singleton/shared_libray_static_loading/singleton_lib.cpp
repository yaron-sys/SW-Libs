#include <iostream> // std

#include "singleton.hpp"
#include "classdef.hpp"


extern "C" void print_obj()
{
    using namespace ilrd;
    
    X* x_obj = Singleton<X>::get_instance();
    
    std::cout << "print_obj(): x_obj->c " << x_obj->c << std::endl;
    std::cout << "print_obj(): x_obj address " << x_obj << std::endl;
}
