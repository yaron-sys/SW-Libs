#include "buse.hpp"
#include "factory.hpp"
#include "handleton.hpp"

class NBDCommand;
struct Args;

#define SINGLE_ARG3(A,B,C) A,B,C
DEFINE_HANDLETON(SINGLE_ARG3(Factory<ilrd::nbd::NBDCommand, unsigned int, ilrd::nbd::Args>))
