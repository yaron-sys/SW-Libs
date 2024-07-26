#include <iostream> // std
#include <dlfcn.h>  // dlopen

#include "factory.hpp"
#include "handleton.hpp"
#include "nbd_project.hpp"
#include "storage_file.hpp"

namespace ilrd{
namespace nbd{

class Print: public NBDCommand{
public:    
    Print(const Args& args): NBDCommand(args){}
   ~Print() = default;
    void do_it()
    {
		size_t i = 0;
		
		while(i < 20){
			std::cout << "Hello " << i << std::endl;
			sleep(1);
			++i;
		}
	}
};

NBDCommand* create_print(const Args& args){
    return new Print(args);
}


extern "C" __attribute__ ((__constructor__))
void add_task()
{
    std::cout << "extern C __attribute__ ((__constructor__))" << std::endl;
    
    using Factory_h = Factory<NBDCommand, unsigned int, Args>;
    Handleton<Factory_h>::get_instance()->add(10, create_print);  
}

}
}
