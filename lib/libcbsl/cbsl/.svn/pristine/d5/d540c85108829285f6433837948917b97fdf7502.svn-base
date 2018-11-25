
#include <netinet/in.h>
#include <string.h>

#include "base_sock_addr.hpp"

namespace cbsl
{
    BaseSockAddr::BaseSockAddr() :
    mySockAddr() ,
    myAddrSize(0)
    {
    }

    BaseSockAddr::~BaseSockAddr()
    {
    }
    
    
    BaseSockAddr::BaseSockAddr( BaseSockAddr &other )
    {
        memcpy( &mySockAddr , &(other.mySockAddr) , other.myAddrSize );
        myAddrSize  = other.myAddrSize;
    }
    
    BaseSockAddr BaseSockAddr::operator=( BaseSockAddr &other )
    {
        memcpy( &mySockAddr , &(other.mySockAddr) , other.myAddrSize );
        myAddrSize  = other.myAddrSize;
        return *this;
    }

    size_t BaseSockAddr::getRawSocketAddr( void *dest , size_t destLen ) const
    {
        if ( destLen<myAddrSize ) {
            return 0;
        }
        
        memcpy( dest , &mySockAddr , myAddrSize );
        return myAddrSize;
    }
    

}; /* namespace cbsl */
