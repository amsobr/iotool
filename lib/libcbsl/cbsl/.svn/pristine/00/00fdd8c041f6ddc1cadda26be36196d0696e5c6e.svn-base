
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include "tcp_connection.hpp"
#include "base_sock_addr.hpp"

namespace cbsl
{

TcpConnection::TcpConnection() :
NetworkConnection() ,
mySockFd(0) ,
myConnected(false)
{
}

TcpConnection::~TcpConnection()
{
    close();
}

Result cbsl::TcpConnection::connect(cbsl::BaseSockAddr const *srvAddr , int flags)
{
    if ( myConnected ) {
        ::close(mySockFd);
    }
    
    mySockFd    = socket(AF_INET,SOCK_STREAM,0);
    if ( mySockFd==-1 ) {
        return OK;
    }

    if ( (flags&NetworkConnection::FLAG_NON_BLOCKING)!=0 ) {
        int origFlags   = fcntl(mySockFd,F_GETFL, 0);
        if ( origFlags==-1 ) {
            return ERR_IO;
        }
    
        fcntl(mySockFd,F_SETFL, origFlags | O_NONBLOCK);
    }
    
    struct sockaddr sockAddr;
    socklen_t addrLen  = srvAddr->getRawSocketAddr(&sockAddr,sizeof(sockAddr));
    
    int res     = ::connect( mySockFd , &sockAddr , addrLen );
    if ( res==-1 ) {
        return ERR_IO;
    }
    
    myConnected = true;
    return OK;
}


Result TcpConnection::close()
{
    if ( !myConnected ) {
        return ERR_BAD_STATE;
    }
    ::close(mySockFd);
    myConnected = false;
    return OK;
}

bool TcpConnection::isConnected() const
{
    return myConnected;
}
    

size_t TcpConnection::countRxBytes()
{
    if ( !myConnected ) {
        return 0;
    }
    int numBytes    = 0;
    ioctl(mySockFd,FIONREAD,&numBytes);
    return numBytes;
}
    

size_t TcpConnection::write( void const *buf , size_t length )
{
    if ( !myConnected ) {
        return 0;
    }
    
    ssize_t cnt   = ::write( mySockFd , buf , length );
    return cnt<0 ? 0 : cnt;
}
    

size_t TcpConnection::read( void *buf , size_t maxLength )
{
    if ( !myConnected ) {
        return 0;
    }
    
    ssize_t cnt       = ::read( mySockFd , buf , maxLength );
    return cnt<0 ? 0 : cnt;
}
    

}; /* namespace cbsl */

