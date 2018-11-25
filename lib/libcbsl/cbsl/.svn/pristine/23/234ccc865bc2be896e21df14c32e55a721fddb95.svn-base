#ifndef BASE_SOCK_ADDR_H
#define BASE_SOCK_ADDR_H

#include <netinet/in.h>

namespace cbsl
{

class BaseSockAddr
{

    
protected:
    struct sockaddr mySockAddr;
    size_t          myAddrSize;
    
    BaseSockAddr();
    
public:
    
    virtual ~BaseSockAddr();
    
    BaseSockAddr( BaseSockAddr &other );
    
    BaseSockAddr operator=( BaseSockAddr &other );
    
    size_t getRawSocketAddr( void *dest , size_t destLen ) const;
    
}; /* class BaseSockAddr */

}; /* namespace cbsl */



#endif /* ! defined( BASE_SOCK_ADDR_H ) */