#include <iostream>

#include "nbd_project.hpp"

using namespace ilrd;

int main(int argc, char *argv[])
{
    // Check for required arguments
    if (argc < 2) {
        printf("Usage: sudo + device file (for example: /dev/nbd0)\n");
        exit(1);
    }
    
    nbd::BDproxy nbd(argv[1],
					 128 * 1024 * 1024,
					 "/home/batman42/tests/test2",
					 "/home/batman42/Desktop/monitor");
    
    std::cout << "Network block device is running..." << std::endl;
}
