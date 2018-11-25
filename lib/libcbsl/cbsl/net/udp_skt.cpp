#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <net/if_arp.h>
#include <sys/ioctl.h>

#include "../thread.hpp"
#include "udp_skt.hpp"
// #include "nwk_print.h"

/* TODO: Make these a parameter */
#define UDP_PORT_SERVER 5000
#define UDP_PORT_LOCAL 5001

namespace cbsl {

UdpSkt::UdpSkt( Config const *cfg ) :
Thread()                    ,
mySockFd(0)                 ,
mySktOpen(false)            ,
myStop(false)               ,
myCfg()
{
    memcpy( &myCfg , cfg , sizeof(myCfg) );
    /* protect ourselves from malicious/invalid configs... */
    myCfg.boundIface[ sizeof(myCfg.boundIface)-1 ]    = '\0';
}

UdpSkt::~UdpSkt()
{
    if ( mySktOpen ) {
        close(mySockFd);
    }
}


bool UdpSkt::init()
{
    int ret;

    mySockFd    = socket( PF_INET , SOCK_DGRAM , IPPROTO_UDP );
    if ( mySockFd== -1 ) {
        int e(errno);
        fprintf( stderr , "PLC UDP: Unable to create socket. errno=%d\n" , e );
        return false;
    }

    /* Restrict outbound packets to a specific interface if boundInterface
     * parameter is non-empty
     */
    if ( myCfg.boundIface[0]!='\0' ) {
            ret = setsockopt(
                            mySockFd                        ,
                            SOL_SOCKET                      ,
                            SO_BINDTODEVICE                 ,
                            myCfg.boundIface                ,
                            strlen(myCfg.boundIface)
                  );

        if ( ret==-1 ) {
            int e(errno);
            fprintf( stderr , "PLC UDP: Unable to bind socket to %s. errno=%d\n" , myCfg.boundIface , e );
            close(mySockFd);
            return false;
        }
    }


    if ( myCfg.flags&Config::FLAG_BCAST ) {
        int enBcast(1);
        ret = setsockopt( mySockFd , SOL_SOCKET , SO_BROADCAST , (void*)&enBcast , sizeof(enBcast) );
        if ( ret==-1 ) {
            int e(errno);
            fprintf( stderr , "PLC UDP: BCAST is flagged, but failed to enable it. errno=%d\n" , e );
            close(mySockFd);
            return false;
        }
    }

    /* bind only if server option is enabled. Otherwise there's no need to */
    if ( myCfg.flags&Config::FLAG_SERVER ) {
        struct sockaddr_in boundAddr;
        memset( &boundAddr , 0 , sizeof(boundAddr) );
        boundAddr.sin_family        = AF_INET;
        boundAddr.sin_addr.s_addr   = htonl( INADDR_ANY );
        boundAddr.sin_port          = htons( myCfg.boundPort );

        ret = bind( mySockFd , (struct sockaddr*) &boundAddr, sizeof(boundAddr) );
        if ( ret==-1 ) {
            int e(errno);
            fprintf( stderr , "PLC UDP: socket bind failed. local port=%d errno=%d\n" , myCfg.boundPort , e );
            close(mySockFd);
            return false;
        }
    }


//     // Set socket to reuse
//     bool reuse = 1;
//     ret = setsockopt( sock_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(bool) );
//     if ( ret < 0 ) {
//         log_error( logctx.halnwk ,  "setsockopt() failed\n" );
//     //  perror("setsockopt() enable reuse failed");
//         //goto err_label;
//     }
    mySktOpen       = true;
    return true;
} /* UdpSkt::init() */


bool UdpSkt::startServer()
{
    if ( myCfg.flags&Config::FLAG_SERVER ) {
        if ( myCfg.rxCb!=0 ) {
            start();
            return true;
        }
        else {
            fprintf( stderr , "UdpSkt: No callback handler defined. Cannot start server.\n" );
            return false;
        }
    }
    else {
        fprintf( stderr , "UdpSkt: Server option is disabled. Cannot start server.\n" );
        return false;
    }
}

void UdpSkt::stopServer()
{
    myStop  = true;
    //stop();
}


void UdpSkt::run(void)
{
    fprintf( stderr , "UDP SKT: worker thread started...\n" );

    while( !myStop ) {
        uint8_t     rxBuf[16385];
        
        struct sockaddr_in srcAddr;
        memset( &srcAddr , 0 , sizeof(srcAddr) );
        
        socklen_t src_address_size(sizeof(srcAddr));
        ssize_t rxLen   = recvfrom( mySockFd , rxBuf , sizeof(rxBuf) , 0 ,
                                    reinterpret_cast<struct sockaddr*>(&srcAddr),
                                    &src_address_size );
        if ( rxLen==-1 ) {
            int e(errno);
            fprintf( stderr , "PLC UDP: Error reading from socket. errno=%d\n" , e );
            continue;
        }
        if ( rxLen==0 ) {
            fprintf( stderr , "PLC UDP: Shutdown requested. Bailing out.\n" );
            break;
        }
        
        
        MsgData msgData;
        msgData.srcIp   = ntohl(srcAddr.sin_addr.s_addr);
        msgData.srcPort = ntohs(srcAddr.sin_port);
        msgData.buf     = rxBuf;
        
        
        msgData.len     = rxLen;
        
//         char strIp[32];
//         nwk_print_ip( msgData.srcIp , strIp , sizeof(strIp) );
//         char strBuf[8192];
//         hexdump( strBuf , sizeof(strBuf) , rxBuf , rxLen , 8 );
//         fprintf( stderr , "UDP SKT: Received message from %s. len=%u, BUF:\n%s\n" , strIp , msgData.len , strBuf );
        
        (*myCfg.rxCb)( myCfg.rxCbArg , &msgData );
//         fprintf( stderr , "UDP SKT: callback returned....\n" );
    }
    
    fprintf( stderr , "PLC UDP: stop request. thread bailing out...\n" );
} /* UdpServer::run() */


size_t UdpSkt::sendMsg( uint32_t ipAddr , unsigned int dport , void const *buf , size_t len )
{
    ssize_t ret;

    struct sockaddr_in dest;
    memset( &dest , 0 , sizeof(dest) );

    dest.sin_family         = AF_INET;
    dest.sin_addr.s_addr    = htonl(ipAddr);
    dest.sin_port           = htons(dport);

    ret = sendto( mySockFd , buf , len , 0 , (struct sockaddr*)&dest , sizeof(dest) );
    if ( ret==-1 ) {
        int e(errno);
        fprintf( stderr , "UdpSkt Failed to transmit data to host 0x%08x:%d. errno=%d\n" , ipAddr , dport , e );
        return -1;
    }

    return ret;
} /* UdpSkt::sendMsg() */

}; /* namespace Wcommon */


