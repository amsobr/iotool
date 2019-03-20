#include <string>
#include <thread>
#include <list>
#include <mutex>

#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/IPAddress.h>
#include <Poco/Net/StreamSocket.h>
#include <Poco/Net/SocketAddress.h>
#include <Poco/Util/Timer.h>
#include <Poco/Logger.h>
#include <Poco/Format.h>
#include <Poco/NumberFormatter.h>

#include <common/board.hpp>
#include <common/adc.hpp>

#include "tcp_data_server.hpp"

using Poco::Logger;
using Poco::Message;
using Poco::format;
using Poco::Timestamp;
using namespace Poco::Net;
using namespace Poco::Util;
using namespace std;

TcpDataServer::TcpDataServer( unsigned int port ) :
myBoundAddress("0.0.0.0") ,
myBoundPort(port) ,
myAcceptorThread() ,
myDispatcherThread() ,
myStreams() ,
myMutex() ,
myTerminate(false) ,
mySocket(SocketAddress(IPAddress(myBoundAddress),myBoundPort)) ,
logger( Poco::Logger::get("iotool") )
{

}

    
    
TcpDataServer::~TcpDataServer()
{
    if ( myAcceptorThread.joinable() || myDispatcherThread.joinable() ) {
        stop();
        myAcceptorThread.join();
        myDispatcherThread.join();
    }
}

void TcpDataServer::processBucket( DataBucket const &db )
{
    logger.debug( Poco::format("Incoming bucket: tag=\"%s\" timestamp=%s #dataPoints=%z",db.name,db.isoTimestamp(),db.dataPoints.size()) );
    std::lock_guard<std::mutex> lock(myMutex);
    /* NB: it is safe to notify first because the lock is being held... */
    /* TODO: make sure that the notify mechanism is
     * not racy...
     */
    if ( myQueuedBuckets.empty() ) {
        logger.debug( "Waking up dispatcher loop..." );
        myCondition.notify_all();
    }
    myQueuedBuckets.push_back(DataBucket(db));
}

void TcpDataServer::acceptorLoop()
{
    logger.debug("Timer started. Entering event loop.");
    while( !myTerminate ) {
        try { 
            logger.information( "TCP Data Server: Waiting for new client...");
            SocketAddress clientAddress;
            StreamSocket stream(mySocket.acceptConnection(clientAddress));
            logger.information( format("TCP Data Server: New client - %s",clientAddress.toString() ) );
            /* Disable the receiving channel, as we just need
             * to send data.
             * Hopefuly allows us to conserve memory from the RX
             * backlog...
             */
            stream.shutdownReceive();
            myMutex.lock();
            myStreams.push_back(stream);
            myMutex.unlock();
        }
        catch ( exception e ) {
            logger.warning( "Exception in acceptor loop:" );
        }
    }
    logger.information("TCP Data Server: Acceptor loop terminated.");
}

void TcpDataServer::dispatcherLoop()
{
 
    logger.debug("TCP Data Server: Dispatcher thread started.");
    while( true ) {
        /* we'll need to keep the lock held (while active) for
            * essentially two reasons:
            *  1. concurrent access to the queue of buckets
            *  2. concurrent access to the list of established streams
            * New streams or buckets will be inserted while the
            * dispatcher is idle.
            */
        std::unique_lock<std::mutex> lock(myMutex);
        if ( myTerminate ) break;

        if ( myQueuedBuckets.empty() ) {
            logger.trace( "TCP Data Server: waiting for data..." );
            myCondition.wait(lock);
            logger.trace( "TCP Data Server: Dispatcher woke up..." );
        }
        else {
            DataBucket &bucket  = *(myQueuedBuckets.begin());
            
            logger.information( Poco::format("TCP Data Server: handling queued bucket tag=\"%s\" timestamp=%s"
                    ,bucket.name
                    ,bucket.isoTimestamp())
            );
            std::string jsonMsg = "{\n";
            jsonMsg += Poco::format( "    \"name\":\"%s\" ,\n",bucket.name);
            jsonMsg += Poco::format( "    \"timestamp\":\"%s\" ,\n",Poco::NumberFormatter::format(bucket.timestamp.epochMicroseconds()/1000) );
            jsonMsg += "    \"values\": {";
            bool first = true;
            for ( auto dataPoint : bucket.dataPoints ) {
                if ( first ) {
                    first = false;
                    jsonMsg += Poco::format( "\n        \"%s\" : \"%s\"",dataPoint.label(),dataPoint.value() );
                }
                else {
                    jsonMsg += Poco::format( ",\n        \"%s\" : \"%s\"",dataPoint.label(),dataPoint.value() );
                }
            }
            jsonMsg        += "\n    }\n}\n\n";
            int jsonMsgLen  = jsonMsg.length();                
            myQueuedBuckets.erase(myQueuedBuckets.begin());
            
            auto it = myStreams.begin();
            while( it!=myStreams.end() ) {
                Poco::Net::StreamSocket &stream(*it);
                try {
                    int res = stream.sendBytes( jsonMsg.c_str() , jsonMsgLen );
                    if ( res != jsonMsgLen ) {
                        logger.information( "Client appears to be disconnected. Cleaning up..." );
                        it = myStreams.erase(it);
                    }
                    else {                
                        it++;
                    }
                }
                catch ( Poco::Exception const &ex ) {
                    logger.warning( Poco::format("TCP Data Server: caught: %s" , ex.displayText() ) );
                    it = myStreams.erase(it);
                }
                catch (std::exception const &ex ) {
                    logger.error( Poco::format("TCP Data Server: caught std exception - %s",ex.what() ) );
                    it = myStreams.erase(it);
                }
            }
        }
    }
    logger.information( "TCP Data Server: dispatcher terminated..." );
}

void TcpDataServer::start()
{
    if ( myAcceptorThread.joinable() ) {
        logger.warning( "TCP server thread already active?! 0.o Bailing out");
        return;
    }

    logger.information("Launching TCP server worker threads");
    myAcceptorThread    = std::thread(&TcpDataServer::acceptorLoop,this);
    myDispatcherThread  = std::thread(&TcpDataServer::dispatcherLoop,this);
}

void TcpDataServer::stop()
{
    mySocket.close();
    myMutex.lock();
    myTerminate     = true;
    myMutex.unlock();
    myAcceptorThread.join();
    myDispatcherThread.join();
    mySocket.close();
}


