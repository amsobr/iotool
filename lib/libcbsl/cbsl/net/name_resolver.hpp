#ifndef NAME_RESOLVER_HPP
#define NAME_RESOLVER_HPP

#include<stdio.h> //printf
#include<string.h> //memset
#include<sys/socket.h>
#include<errno.h> //For errno - the error number
#include<netdb.h> //hostent
#include<arpa/inet.h>

#include "ip_addr.hpp"


namespace cbsl
{

namespace NameResolver 
{
 
    extern IpAddr resolve4( char const *hostname );
    
    extern IpAddr resolve6( char const *hostname );

}; /* NameResolver NameResolver */

}; /* namespace NameResolver */



#endif /* !defined( NAME_RESOLVER_HPP ) */
