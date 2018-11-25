#ifndef IP_ADDR_HPP
#define IP_ADDR_HPP

#include <sys/socket.h>
#include <string.h>
#include <assert.h>

namespace cbsl
{
    
class IpAddr {
    
private:
    struct sockaddr mySockAddr;
    socklen_t       mySockAddrLen;
    
public:
    IpAddr() :
    mySockAddr()  ,
    mySockAddrLen(0)
    {
    }
    
    IpAddr( IpAddr const &other )
    {
        assert( other.mySockAddrLen<=sizeof(mySockAddr) );
        
        memcpy(&mySockAddr,&(other.mySockAddr),other.mySockAddrLen);
        mySockAddrLen   = other.mySockAddrLen;
    }

    
    IpAddr operator=( IpAddr const &other )
    {
        assert( other.mySockAddrLen<=sizeof(mySockAddr) );
        
        memcpy(&mySockAddr,&(other.mySockAddr),other.mySockAddrLen);
        mySockAddrLen   = other.mySockAddrLen;
        return *this;
    }
    
    /** NON-VIRTUAL destructor.
     * Please don't inherit from this class to avoid obscure behaviour that
     * may be caused by copy-construction and assignment operations.
     */
    ~IpAddr()
    {
    }
    
    void loadFromAddr( struct sockaddr const *sockAddr , socklen_t sockLen )
    {
        assert( sockLen<=sizeof(mySockAddr) );
        
        memcpy(&mySockAddr,sockAddr,sockLen);
        mySockAddrLen   = sockLen;
    }
    
    socklen_t getSockAddr( void *dst , socklen_t maxLen )
    {
        memcpy( dst , &mySockAddr , maxLen );
        return maxLen<=mySockAddrLen ? maxLen : mySockAddrLen;
    }

    void print( char *dst , size_t len );
    
}; /* class IpAddr */

}; /* namespace cbsl */


#endif /* ! defined( IP_ADDR_HPP ) */
