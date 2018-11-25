#ifndef SSL_CONNECTION_SSL
#define SSL_CONNECTION_SSL

#include <openssl/ssl.h>

#include "network_connection.hpp"
#include "../result.hpp"


namespace cbsl
{

    
class SslConnection : public NetworkConnection
{
private:
    SSL_CTX    *mySslCtx;
    SSL        *mySsl;
    
    int         mySockFd;
    bool        myConnected;


public:
    SslConnection();
    ~SslConnection();
    
    static void initLibrary();
    
    virtual Result connect( BaseSockAddr *sock , int flags=0 );
    
    virtual Result close();
    
    virtual bool isConnected() const;
    
    virtual size_t countRxBytes();
    
    virtual size_t write( void const *buf , size_t len );
    
    virtual size_t read( void *buf , size_t maxLen );
    
    void printPeerCert();
}; /* class SslConnection */

}; /* namespace cbsl */

#endif /* !defined( SSL_CONNECTION_SSL ) */
