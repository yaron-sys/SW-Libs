#include <iostream> // std
#include <dlfcn.h>  // dlopen

#include "a.hpp"
#include "handleton.hpp"

extern "C" void print_obj()
{
    using namespace ilrd;
    
    X* x_obj = Handleton<X>::get_instance();
    
    std::cout << "print_obj(): x_obj->c " << x_obj->c << std::endl;
    std::cout << "print_obj(): x_obj address " << x_obj << std::endl;
}
