#ifndef UDP_SOCKET_HPP_
#define UDP_SOCKET_HPP_

#include <stdexcept>    //exception

#include <list>
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


struct UDPRecipient {
public:
    UDPRecipient(struct sockaddr_storage client_addr, const std::string& ip, const std::string& port)
     : m_ip(ip), m_port(port), m_addr(client_addr), m_addr_len(sizeof(m_addr)) {}  
           
    const std::string& get_ip() const { return m_ip; }
    const std::string& get_port() const { return m_port; }
    struct sockaddr_storage* get_sockaddr_addr() { return &m_addr; }
    socklen_t get_sockaddr_len() { return m_addr_len; }

private:       
    const std::string m_ip;
    const std::string m_port;
    
    struct sockaddr_storage m_addr;
    socklen_t m_addr_len;
};


class UDPSocket{
public:
    explicit UDPSocket(const char* port = "1234"); // @server side: host-ip is nullptr
    explicit UDPSocket(const char* host_ip, const char* port = "1234"); // @client side
   ~UDPSocket();
   
	ssize_t send(const char* buf,size_t len, const std::string& dest_ip="", const std::string& dest_port="");
	ssize_t receive(char* buf,size_t len,std::string* out_src_ip=0, std::string* out_src_port=0);
	int get_native_handle();
	
private:	
	const char* m_serv_ip;
    const char* m_serv_port;
    int m_sockfd;
    struct addrinfo *m_server_info;
    std::list<UDPRecipient> m_recipient;
    
    void set_native_handle();
    std::list<UDPRecipient>::iterator find_recipient(const std::string& ip, const std::string& port);
    std::string extract_ip(struct sockaddr_storage& src_addr);
    std::string extract_port(struct sockaddr_storage& src_addr);
    inline void* get_in_addr(struct sockaddr* sa); // returns src address, regardless of IPv4/6
};

//******************************************************************************
UDPSocket::UDPSocket(const char* port)
: m_serv_ip(nullptr), m_serv_port(port), m_sockfd(0), m_server_info(nullptr)
{
	set_native_handle(); // set m_sockfd & m_server_info
}

UDPSocket::UDPSocket(const char* host_ip, const char* port)
: m_serv_ip(host_ip), m_serv_port(port), m_sockfd(0), m_server_info(nullptr)
{
	set_native_handle(); // set m_sockfd & m_server_info
}
//******************************************************************************
UDPSocket::~UDPSocket()
{
	close(m_sockfd);
}
//******************************************************************************
void UDPSocket::set_native_handle()
{
    int rv = 0;
    struct addrinfo* it = nullptr;
    struct addrinfo* servinfo = nullptr;
	struct addrinfo hints;
    memset(&hints, 0, sizeof hints);
    
    hints.ai_family = AF_UNSPEC;    // ipv4 / ipv6
    hints.ai_socktype = SOCK_DGRAM; // UDP
    if(m_serv_ip == nullptr){
		hints.ai_flags = AI_PASSIVE; // use my IP for server
	}
	
    if ((rv = getaddrinfo(m_serv_ip, m_serv_port, &hints, &servinfo)) != 0) {
        std::cerr << "getaddrinfo: " << gai_strerror(rv) << std::endl;
        return;
    }
    
    // loop through all the results and make a socket
    for(it = servinfo; it != nullptr; it = it->ai_next) {
        if ((m_sockfd = socket(it->ai_family, it->ai_socktype, it->ai_protocol)) == -1) {
            perror("talker: socket");
            continue;
        }    
        
        if ((m_serv_ip == nullptr) && (bind(m_sockfd, it->ai_addr, it->ai_addrlen) == -1)) {
            close(m_sockfd);
            perror("listener: bind");
            continue;
        }
        break;
    }
    if (it == nullptr) {
        std::cerr << "failed to bind socket" << std::endl;
    }
    freeaddrinfo(servinfo);
    m_server_info = it;
}

//******************************************************************************
int UDPSocket::get_native_handle()
{
    return m_sockfd;
}
//******************************************************************************
ssize_t UDPSocket::send(const char* buf, size_t len, const std::string& dest_ip, const std::string& dest_port)
{
	ssize_t numbytes = 0;
	
	if(m_serv_ip == nullptr) {
		auto iter = find_recipient(dest_ip, dest_port);
		if (iter != m_recipient.end()) {
			numbytes = sendto(m_sockfd, buf, len, 0,
							  (struct sockaddr *)(iter->get_sockaddr_addr()),
							  iter->get_sockaddr_len());
		}
	}
	
	else{
		numbytes = sendto(m_sockfd, buf, len, 0, m_server_info->ai_addr, m_server_info->ai_addrlen);
	}
	
	if(numbytes == -1){
	    perror("talker: sendto");
	}
	
	return numbytes;
}
//******************************************************************************
ssize_t UDPSocket::receive(char* buf,size_t len,
						   std::string* out_src_ip, std::string* out_src_port)
{	
	struct sockaddr_storage their_addr_tmp;
    socklen_t addr_len_tmp = sizeof(their_addr_tmp);
    
	ssize_t numbytes = recvfrom(m_sockfd, buf, len , 0, (struct sockaddr*)&their_addr_tmp, &addr_len_tmp);
	if(numbytes == -1) {
		perror("talker: recvfrom");
	}
	
	if(m_serv_ip == nullptr) {	    
		std::string src_ip(extract_ip(their_addr_tmp));
		std::string src_port(extract_port(their_addr_tmp));
		
		if (find_recipient(src_ip, src_port) == m_recipient.end()) {
			m_recipient.push_front(UDPRecipient(their_addr_tmp, src_ip, src_port));
		}
		
		if (out_src_ip != 0) {
			out_src_ip->assign(src_ip);
		}
		if (out_src_port != 0) {
			out_src_port->assign(src_port);
		}
	}

	return numbytes;
}
//******************************************************************************
std::list<UDPRecipient>::iterator UDPSocket::find_recipient(const std::string& ip, const std::string& port)
{
    std::list<UDPRecipient>::iterator iter = m_recipient.begin();
    while (iter != m_recipient.end()) {
        if ((iter->get_ip().compare(ip) == 0) && (iter->get_port().compare(port) == 0)) {
            break;
        }
        ++iter;
    }
    
    return iter;
}

//******************************************************************************
std::string UDPSocket::extract_ip(struct sockaddr_storage& src_addr)
{
    char src_ip_buffer[INET6_ADDRSTRLEN];
    inet_ntop(src_addr.ss_family,
              get_in_addr((struct sockaddr *)&src_addr),
              src_ip_buffer, 
              sizeof src_ip_buffer);
    
    return src_ip_buffer;
}

//******************************************************************************
std::string UDPSocket::extract_port(struct sockaddr_storage& src_addr)
{
    return std::to_string(ntohs(((struct sockaddr_in*)&src_addr)->sin_port));
}

//******************************************************************************
void* UDPSocket::get_in_addr(struct sockaddr* sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}
//******************************************************************************

} // namespace nbd
} // namespace ilrd

#endif // UDP_SOCKET_HPP_
