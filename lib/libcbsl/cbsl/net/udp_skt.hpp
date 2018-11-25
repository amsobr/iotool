/**
 * \file   udp_server.hpp
 * \Author Antonio Oliveira (antonio.oliveira@withus.pt)
 * \date   January 2014
 * \brief  Provides class to transmit and receive over an UDP socket
 */
#ifndef UDP_SERVER_HPP
#define UDP_SERVER_HPP

#include <stddef.h>
#include <stdint.h>

#include "../nocopy.hpp"
#include "../thread.hpp"

namespace cbsl {

class UdpSkt : public cbsl::Thread {
public:

struct MsgData {
    uint32_t        srcIp;
    unsigned int    srcPort;
    void const     *buf;
    size_t          len;
};

typedef void (rx_cb_fn)( void * , MsgData const * );

struct Config {
    typedef enum  {
        LISTEN_LO       ,
        LISTEN_ANY      ,
    } listen_mode_t;

    typedef enum {
        FLAG_SERVER         = 0x00000001    ,
        FLAG_BCAST          = 0x00000002    ,
    } flag_t;

    char            boundIface[64];
    unsigned int    boundPort;
    listen_mode_t   listenMode;
    rx_cb_fn       *rxCb;
    void           *rxCbArg;
    uint32_t        flags;
}; /* struct UdpSktCfg */

private:
    WC_DISABLE_COPY(UdpSkt);    
    UdpSkt();
    
    
    int                 mySockFd;
    bool                mySktOpen;
    bool                myStop;

    Config              myCfg;
    
    /** \brief worker thread
     * This thread  may only be invoqued, from startServer, if
     * FLAG_EN_SERVER is active in the configuration
     */    
    virtual void run();
    
     /* prevent clients from launching the worker directly. We need to
      * explicitly control worker startup, depending on the configuration.
      */
    virtual void start() { cbsl::Thread::start(); }
    
    
public:
    UdpSkt(Config const *cfg);
    virtual ~UdpSkt();
    
    /** \brief Initialize the socket, using configuration passed to the
     * constructor
     */
    bool init();

    inline int fd() const { return mySockFd; };
    
    /** \brief Launch the server that listens on the socket.
     * \return Returns true if the worker is launched or false in case the
     * configuration has the server flag disabled.
     */
    bool startServer();
    
    /** \brief Stops a running server, is any
     */
    void stopServer();

    /** \brief Send a message to a host using UDP
     * \details Attempte to transmit a message to the destination host.
     * \param ipAddr    IPv4 address of the destination host
     * \param dport     Destination port on the destination host
     * \param buf       Pointer to location of the datagram to be sent
     * \param len       Size of the data to send.
     * \return  Returns the total number of bytes written into the socket or
     *          -1 in case of error.
     */
    size_t sendMsg( uint32_t ipAddr , unsigned int dport , void const *buf , size_t len );
    
}; /* class UdpServer */

}; /* namespace Wcommon */

#endif /* !defined(UDP_SERVER_HPP) */

