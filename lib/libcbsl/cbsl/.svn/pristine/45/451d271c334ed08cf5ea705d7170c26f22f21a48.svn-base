#ifndef NETWORK_CONNECTION_HPP
#define NETWORK_CONNECTION_HPP

#include <stdlib.h>

#include "../result.hpp"
#include "../nocopy.hpp"

namespace cbsl
{
    
    class BaseSockAddr;

class NetworkConnection
{
private:
    CBSL_NO_COPY(NetworkConnection);
    
protected:
    NetworkConnection()
    {
    }
    
public:
    
    enum {
        FLAG_NON_BLOCKING   = 0x00000001 ,
        
    };
    
    virtual ~NetworkConnection()
    {
    }
    
    /** Connect to a socket, specified by \a sock.
     * Use the \a flags to specify special behaviours that shall be
     * taken by the connection.
     * 
     * \p
     * If a connection is already open, it shall be closed automatically
     * before the new one is opened.
     * \p
     * If a previous connection was closed and the new one failed to connect
     * the corresponding result code shall be returned. The old connection
     * won't be usable anymore as well.
     */
    virtual Result connect( BaseSockAddr *sock , int flags=0 )    = 0;
    
    /** close the current connection.
     * \p An attempt to close a non-connected connection returns a BAD_STATE
     * result.
     */
    virtual Result close()      = 0;
    
    /** Check if a connection is established */
    virtual bool isConnected() const  = 0;
    
    /** Count the number of available bytes on the connection.
     * \p
     * On disconnected connections shall return 0
     * \p
     * The result of this call is not guaranteed on all platforms.
     */
    virtual size_t countRxBytes()   = 0;
    
    /** Write the contents of a buffer into the connection and return the
     * number of bytes actually written.
     */
    virtual size_t write( void const *buf , size_t len )  = 0;
    
    /** Read from the connections buffer into a location specified by the
     * caller. And return the number of bytes actually read.
     * \p
     * The capacity of the destination buffer (\a buf) shall never be exceeded
     * \p
     * When unconnected, this call shall produce no effect at all.
     */
    virtual size_t read( void *buf , size_t maxLen )    = 0;
}; /* class NetworkConnection */

}; /* namespace cbsl */

#endif /* !defined(NETWORK_CONNECTION_HPP) */
