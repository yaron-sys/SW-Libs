#include <iostream>
#include <string.h>
#include <limits>

#include "udp_socket.hpp"

using namespace ilrd;

#define SIZE 20

/*******************************************************************************
 * 									server
 ******************************************************************************/
int main(int argc, char *argv[])
{
    // Check for required arguments
    if (argc < 2) {
        printf("Usage: enter server port\n");
        exit(1);
    }
    
    int numbytes = 0;
    size_t counter = 0;
    char* buf = new char[SIZE];
    nbd::UDPSocket udp_server(argv[1]);
    std::string out_src_ip;
    std::string out_src_port;
    
    while(true) {
		std::cout << "Server: waiting to recvfrom..." << std::endl;
		if((numbytes = udp_server.receive(buf, SIZE, &out_src_ip, &out_src_port)) == -1){
			break;
		}
	    
	    if((strcmp(buf,"ping\n") == 0) || (strcmp(buf,"ping") == 0)) {
			const char* answer = "pong\n";
			if(udp_server.send(answer, strlen(answer)+1, out_src_ip, out_src_port) == -1){
				break;
			}
        }
   
        else{
			if(udp_server.send("", 1, out_src_ip, out_src_port) == -1){
				break;
			}
        }	  
        std::cout << "Server: got packet " << counter << " from " << out_src_ip << " port " << out_src_port << std::endl;
        std::cout << "Server: packet is " << numbytes-1 << " bytes long." << std::endl;
        buf[numbytes-1] = '\0';
        std::cout << "Server: packet contains \"" << buf << "\"" << std::endl << std::endl;  
        ++counter;
	}

    delete(buf);
}
