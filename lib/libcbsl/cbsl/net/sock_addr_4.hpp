#ifndef SOCK_ADDR_4_HPP
#define SOCK_ADDR_4_HPP

#include <stdint.h>

#include "base_sock_addr.hpp"
#include "ip_addr.hpp"

namespace cbsl
{
    
class SockAddr4 : public BaseSockAddr
{
public:
    SockAddr4( char const *ipAddr , unsigned short port );
    SockAddr4( IpAddr ipAddr , unsigned short port );
    
    ~SockAddr4();
    
    static BaseSockAddr SockAddr4Any( unsigned short port );

    
}; /* class SockAddr4 */

}; /* namespace cbsl */


#endif /* !defined(SOCK_ADDR_4_HPP) */
