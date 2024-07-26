
#ifndef UDP_SOCKET_HPP_
#define UDP_SOCKET_HPP_

#include <stdexcept>    //exception
//#include "nbd.hpp"  //nbd

#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <iostream>

namespace ilrd{
namespace nbd {

class UDPSocket{
public:
    UDPSocket(const char* host_ip, const char* port);
   ~UDPSocket();
	ssize_t send(char* buf,size_t len);
	ssize_t receive(char* buf,size_t len);
	int get_native_handle();
	
private:
	const char* m_ip;
    const char* m_port;
    int m_sockfd;
    struct addrinfo *m_server_info;
};

UDPSocket::UDPSocket(const char* host_ip, const char* port)
: m_ip(host_ip), m_port(port)
{
	m_sockfd = get_native_handle();
}

UDPSocket::~UDPSocket()
{
	close(m_sockfd);
}

int UDPSocket::get_native_handle()
{
    struct addrinfo hints;
    struct addrinfo *servinfo;
    struct addrinfo *it;
    int rv;

    memset(&hints, 0, sizeof hints);
    
    hints.ai_family = AF_UNSPEC;    // ipv4 / ipv6
    hints.ai_socktype = SOCK_DGRAM; // UDP
    if ((rv = getaddrinfo(m_ip, m_port, &hints, &servinfo)) != 0) {
        std::cerr << "getaddrinfo: " << gai_strerror(rv) << std::endl;
        return 1;
    }
    
    // loop through all the results and make a socket
    for(it = servinfo; it != nullptr; it = it->ai_next) {
        if ((m_sockfd = socket(it->ai_family, it->ai_socktype, it->ai_protocol)) == -1) {
            perror("talker: socket");
            continue;
        }
        break;
    }
    m_server_info = it;
    
    return m_sockfd;
}

ssize_t UDPSocket::send(char* buf, size_t len)
{
	ssize_t numbytes = sendto(m_sockfd, buf, len, 0, m_server_info->ai_addr, m_server_info->ai_addrlen);
	if(numbytes == -1) {
	    perror("talker: sendto");
	    exit(1);
	}
	
	return numbytes;
}

ssize_t UDPSocket::receive(char* buf,size_t len)
{
	struct sockaddr_storage their_addr;
    socklen_t addr_len;
	
	ssize_t numbytes = recvfrom(m_sockfd, buf, len , 0, (struct sockaddr *)&their_addr, &addr_len);
	if(numbytes == -1) {
		
	}
	
	return numbytes;
}


} // namespace nbd
} // namespace ilrd

#endif // UDP_SOCKET_HPP_
