#include <iostream> // std

#include "singleton.hpp"
#include "classdef.hpp"

extern "C" void print_obj();

int main()
{
    using namespace ilrd;
    
    X* x_obj = Singleton<X>::get_instance();
    
    std::cout << "main(): x_obj->c " << x_obj->c << std::endl;
    std::cout << "main(): x_obj address " << x_obj << std::endl;
    
    // call to print_obj() via static loading
    print_obj();
    
    return 0;
}
