

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/fcntl.h>
#include <resolv.h>
#include <netdb.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/conf.h>



#include "network_connection.hpp"
#include "../result.hpp"

#include "ssl_connection.hpp"
#include "base_sock_addr.hpp"


namespace cbsl
{


void SslConnection::initLibrary()
{
    SSL_library_init();
    SSL_load_error_strings();
    OPENSSL_config(NULL);
}

SslConnection::SslConnection()
{
    
}

SslConnection::~SslConnection()
{
}

Result SslConnection::connect( BaseSockAddr *sock , int flags )
{
    if ( isConnected() ) {
        close();
    }
    
    struct sockaddr sockAddr;
    mySockFd        = socket(PF_INET,SOCK_STREAM,0);

    if ( (flags&NetworkConnection::FLAG_NON_BLOCKING)!=0 ) {
        int origFlags   = fcntl(mySockFd,F_GETFL, 0);
        if ( origFlags==-1 ) {
            printf( "ERROR getting original flags.\n" );
            ::close(mySockFd);
            return ERR_IO;
        }
        fcntl(mySockFd,F_SETFL, origFlags | O_NONBLOCK);
    }

    //bzero(&addr, sizeof(addr));
    sock->getRawSocketAddr(&sockAddr,sizeof(sockAddr));
    int res = ::connect(mySockFd,(struct sockaddr*)&sockAddr, sizeof(sockAddr));
    if ( res!=0 ) {
        printf( "ERROR connecting to socket: %d\n" , errno );
        ::close(mySockFd);
        return ERR_IO;
    }

    SSL_METHOD const *method  = SSLv23_client_method();  /* Create new client-method instance */
    mySslCtx            = SSL_CTX_new(method);  /* Create new context */
    if ( mySslCtx==0 ) {
        ERR_print_errors_fp(stderr);
        ::close(mySockFd);
        return ERR_IO;
    }
    
    mySsl               = SSL_new(mySslCtx);      /* create new SSL connection state */
    SSL_set_fd(mySsl,mySockFd);    /* attach the socket descriptor */
    if ( SSL_connect(mySsl)!=1 ) {   /* perform the connection */
        ERR_print_errors_fp(stderr);
        SSL_CTX_free(mySslCtx);
        ::close(mySockFd);
        return ERR_IO;
    }
    else {
        myConnected     = true;
    }
    return OK;
}


Result SslConnection::close()
{
    if ( !isConnected() ) {
        return ERR_BAD_STATE;
    }
    
    SSL_free(mySsl);        /* release connection state */
    ::close(mySockFd);
    SSL_CTX_free(mySslCtx);        /* release context */
    myConnected     = false;
}

bool SslConnection::isConnected() const
{
    return myConnected;
}


size_t SslConnection::countRxBytes()
{
    return SSL_pending(mySsl);
}


size_t SslConnection::write( void const *buf , size_t len )
{
    int res     = SSL_write( mySsl , buf , len );
    return res>0 ? res : 0;
}


size_t SslConnection::read( void *buf , size_t maxLen )
{
    int res = SSL_read(mySsl,buf,maxLen);
    return res>0 ? res : 0;
    
}
 

void SslConnection::printPeerCert()
{
    X509 *cert;
    char *line;
 
    cert = SSL_get_peer_certificate(mySsl); /* get the server's certificate */
    if ( cert!=0 ) {
        printf("Server certificates:\n");
        line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
        printf("Subject: %s\n", line);
        free(line);       /* free the malloc'ed string */
        line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
        printf("Issuer: %s\n", line);
        free(line);       /* free the malloc'ed string */
        X509_free(cert);     /* free the malloc'ed certificate copy */
    }
    else {
        printf("No certificates.\n");
    }
}

}; /* namespace cbsl */
