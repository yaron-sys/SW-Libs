#include <iostream>
#include <string.h>
#include <limits>

#include "udp_socket.hpp"

using namespace ilrd;

#define SIZE 50


/*******************************************************************************
 * 									client
 ******************************************************************************/
int main(int argc, char *argv[])
{
    // Check for required arguments
    if (argc < 2) {
        printf("Usage: enter server ip + server port (for example: localhost 12345\n");
        exit(1);
    }
    
    int numbytes = 0;
    char* buf = new char[SIZE];
    nbd::UDPSocket udp_client(argv[1], argv[2]);
    
    while(true) {
		memset(buf, 0, SIZE);
		std::cin.getline(buf, SIZE);

		udp_client.send(buf, strlen(buf)+1);
		
		if((numbytes = udp_client.receive(buf, strlen(buf)+1)) != 0){
			std::cout << buf;
		}
		
		std::cin.clear();
	}
	
    delete(buf);
}
