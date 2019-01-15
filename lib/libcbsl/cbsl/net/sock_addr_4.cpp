
#include <stdint.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "sock_addr_4.hpp"

namespace cbsl
{
    
SockAddr4::SockAddr4( char const *ipAddr , unsigned short port )
: BaseSockAddr()
{
    struct sockaddr_in *ipv4_addr   = (struct sockaddr_in*)&mySockAddr;
    
    ipv4_addr->sin_family           = AF_INET;
    ipv4_addr->sin_port             = htons(port);
    inet_aton(ipAddr,&(ipv4_addr->sin_addr));
    myAddrSize                      = sizeof(struct sockaddr_in);
}


SockAddr4::SockAddr4(IpAddr ipAddr, short unsigned int port)
{
    struct sockaddr_in tmpAddr;
    struct sockaddr_in *ipv4_addr   = (struct sockaddr_in*)&mySockAddr;
    ipv4_addr->sin_family           = tmpAddr.sin_family;
    ipv4_addr->sin_port             = htons(port);
    memcpy( &(ipv4_addr->sin_addr) , &(tmpAddr.sin_addr) , sizeof(ipv4_addr->sin_addr) );
}

    
SockAddr4::~SockAddr4()
{
}


}; /* namespace cbsl */
