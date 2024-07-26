#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define SERVERPORT "4950"   // the port users will be connecting to
#define MAXBUFLEN 5

int main(int argc, char *argv[])
{
	char buf[MAXBUFLEN];
    int sockfd;
    struct sockaddr_storage their_addr;
    socklen_t addr_len;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    int numbytes;
    if (argc != 2) {
        fprintf(stderr,"usage: talker hostname message\n");
        exit(1);
    }
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;    // ipv4 / ipv6
    hints.ai_socktype = SOCK_DGRAM; // UDP
    if ((rv = getaddrinfo(argv[1], SERVERPORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }
    // loop through all the results and make a socket
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("talker: socket");
            continue;
        }
        break;
    }
    
    printf("client socket fd: %d\n", sockfd);
    
    if (p == NULL) {
        fprintf(stderr, "talker: failed to create socket\n");
        return 2;
    }
    int counter = 0;
    while(counter != 3)
    {
    	++counter;
		if ((numbytes = sendto(sockfd, "ping", 5, 0, p->ai_addr, p->ai_addrlen)) == -1) {
		    perror("talker: sendto");
		    exit(1);
		}
		recvfrom(sockfd, buf, 5 , 0, (struct sockaddr *)&their_addr, &addr_len);
		printf("%s %d", buf, counter);
	}
    close(sockfd);
    return 0;
}
