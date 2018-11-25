#ifndef TCP_CLIENT_CONNECTION_HPP
#define TCP_CLIENT_CONNECTION_HPP

#include <stdlib.h>

#include "../nocopy.hpp"
#include "../result.hpp"

#include "network_connection.hpp"


namespace cbsl
{
    class BaseSockAddr;

class TcpConnection : public NetworkConnection
{
private:
    CBSL_NO_COPY(TcpConnection);
    
    int mySockFd;
    bool myConnected;

public:
    TcpConnection();

    virtual ~TcpConnection();
    
    virtual Result connect( BaseSockAddr const *srvAddr , int flags=0 );
    
    virtual Result close();
    
    virtual bool isConnected() const;
    
    virtual size_t countRxBytes();
    
    virtual size_t write( void const *buf , size_t length );
    
    virtual size_t read( void *buf , size_t maxLength );
    
}; /* class TcpClientConnection */

}; /* namespace cbsl */


#endif /* !defined TCP_CLIENT_CONNECTION_HPP */