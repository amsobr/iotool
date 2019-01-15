
#include<stdio.h> //printf
#include<string.h> //memset
#include<sys/socket.h>
#include<errno.h> //For errno - the error number
#include<netdb.h> //hostent
#include<arpa/inet.h>

#include "name_resolver.hpp"
#include "ip_addr.hpp"


namespace cbsl
{

namespace NameResolver 
{
 
IpAddr resolve4( char const *hostname )
{
    struct addrinfo hints , *servinfo, *p;

    memset(&hints, 0, sizeof hints);
    hints.ai_family     = AF_INET;
    hints.ai_socktype   = 0;
 
    int res = getaddrinfo( hostname , 0 , &hints , &servinfo);
    IpAddr ipAddr;
    
    if ( res!=0 || servinfo==NULL ) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(res));
        return ipAddr;
    }
 
    for ( p=servinfo ; p!=NULL ; p=p->ai_next ) {
        ipAddr.loadFromAddr(p->ai_addr,p->ai_addrlen);
        freeaddrinfo(servinfo);
        return ipAddr;
    }
}

IpAddr resolve6( char const *hostname )
{
    struct addrinfo hints , *servinfo, *p;
 
    memset(&hints, 0, sizeof hints);
    hints.ai_family     = AF_INET6;
    hints.ai_socktype   = 0;
 
    int res = getaddrinfo( hostname , 0 , &hints , &servinfo);
    IpAddr ipAddr;
    
    if ( res!=0 || servinfo==NULL ) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(res));
        return ipAddr;
    }
 
    for ( p=servinfo ; p!=NULL ; p=p->ai_next ) {
        ipAddr.loadFromAddr(p->ai_addr,p->ai_addrlen);
        freeaddrinfo(servinfo);
        return ipAddr;
    }
    return ipAddr;
}

}; /* NameResolver NameResolver */

}; /* namespace NameResolver */

