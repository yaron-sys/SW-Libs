#include <iostream> // std
#include <dlfcn.h>  // dlopen

#include "singleton.hpp"
#include "classdef.hpp"

extern "C" void print_obj();

int main()
{
    using namespace ilrd;
    
    X* x_obj = Singleton<X>::get_instance();
    
    std::cout << "main(): x_obj->c " << x_obj->c << std::endl;
    std::cout << "main(): x_obj address " << x_obj << std::endl;
    
    // call to print_obj() via dynamic loading
    void (*func_ptr)(void);
    void *lib = dlopen("./libsing.so", RTLD_LAZY);
    if (lib == nullptr) {
        std::cout << "failed to load dyn lib" << std::endl;
        return 1;
    }
    *(void **)(&func_ptr) = dlsym(lib, "print_obj");
    func_ptr();  // call print_obj()
    
    dlclose(lib);

    return 0;
}
