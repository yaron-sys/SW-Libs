#include <iostream>
#include <string.h>
#include <limits>

#include "udp_socket.hpp"

using namespace ilrd;

#define SIZE 20

int main(int argc, char *argv[])
{
    // Check for required arguments
    if (argc < 2) {
        printf("Usage: enter server ip + server port (for example: localhost 12345\n");
        exit(1);
    }
    
    char* buf = new char[SIZE];
    nbd::UDPSocket udp_obj(argv[1], argv[2]);
    
    while(1) {
		
		memset(buf, 0, SIZE);
		
		std::cin >> buf;
		udp_obj.send(buf, strlen(buf)+1);
		
		if(udp_obj.receive(buf, strlen(buf)+1) != 0){
			std::cout << buf << std::endl;
		}
		
		std::cin.clear();
	}

    delete(buf);
    
}
