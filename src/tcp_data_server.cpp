#include <string>
#include <thread>
#include <list>
#include <mutex>

#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/IPAddress.h>
#include <Poco/Net/StreamSocket.h>
#include <Poco/Net/SocketStream.h>
#include <Poco/Net/SocketAddress.h>
#include <Poco/Util/Timer.h>
#include <Poco/Logger.h>
#include <Poco/Format.h>

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
    logger.debug( Poco::format("Incoming bucket: tag=\"%s\" timestamp=%s #dataPoints=%u",db.tag,db.isoTimestamp(),db.dataPoints.size()) );
    std::lock_guard<std::mutex> lock(myMutex);
    /* NB: it is safe to notify first because the lock is being held... */
    if ( myQueuedBuckets.empty() ) {
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
        try {
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
                
                logger.information( Poco::format("TCP Data Server: handling queued bucket tag=%s timestamp=%s"
                        ,bucket.tag
                        ,bucket.isoTimestamp())
                );
                std::string jsonMsg = "{\n";
                jsonMsg += Poco::format( "    \"label\":\"%s\" ,\n",bucket.tag);
                jsonMsg += Poco::format( "    \"timestamp\":\"%s\"",bucket.isoTimestamp() );
                for ( auto dataPoint : bucket.dataPoints ) {
                    jsonMsg += Poco::format( ",\n    \"%s\" : \"%s\"",dataPoint.label(),dataPoint.value() );
                }
                jsonMsg += "\n}\n";
                myQueuedBuckets.erase(myQueuedBuckets.begin());
                
                for ( auto &stream : myStreams ) {
                    stream.sendBytes( jsonMsg.c_str() , jsonMsg.length() );
                }
            }
        }
        catch ( Poco::Exception const &ex ) {
            logger.error( Poco::format("TCP Data Server: caught exception - %s",ex.displayText()) );
        }
        catch (std::exception const &ex ) {
            logger.error( Poco::format("TCP Data Server: caught exception - %s",ex.what()) );
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


