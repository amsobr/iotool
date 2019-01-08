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

#include <common/board.hpp>
#include <common/adc.hpp>

#include "tcp_server.hpp"

using Poco::Logger;
using Poco::Message;
using Poco::format;
using Poco::Timestamp;
using namespace Poco::Net;
using namespace Poco::Util;
using namespace std;

TcpServer::AcquisitionTask::AcquisitionTask(TcpServer &server) :
self(server)
{
}

TcpServer::AcquisitionTask::~AcquisitionTask() {}

void TcpServer::AcquisitionTask::run()
{
    Logger &logger  = Logger::get("iotool.tcp-server");

    logger.information( "TcpServer AcquisitionTask now iterating...");

    int entryCount = 0;
    string msg("{");
    for ( PeripheralPtr p : self.myBoard->getPeripherals() ) {
        if ( p->getType()==PeripheralType::ADC ) {
            AdcPtr adc  = dynamic_pointer_cast<Adc>(p);
            size_t numChannels  = adc->getNumChannels();
            for ( size_t ch=0 ; ch<numChannels ; ch++ ) {
                double v    = adc->readAnalog(ch);
                char str[128];
                snprintf( str , sizeof(str) , "%s    \"adc%d.%d\" : %.8f " , entryCount==0?"":",\n" , adc->getId() , ch , v );
                msg += str;
                entryCount++;
            }
        }
    }    
    msg += "\n}";

    lock_guard<mutex>(self.myMutex);
    for( auto stream : self.myStreams ) {
        stream.sendBytes(msg.c_str(),msg.size());
    }
}




TcpServer::TcpServer( BoardPtr board , unsigned int port , unsigned int interval ) :
myBoard(board) ,
myBoundAddress("127.0.0.1") ,
myBoundPort(port) ,
mysendInterval(interval) ,
myThread() ,
myStreams() ,
myMutex() ,
myTerminate(false) ,
mySocket(SocketAddress(IPAddress(myBoundAddress),myBoundPort))
{

}

    
    
TcpServer::~TcpServer()
{
    if ( myThread.joinable() ) {
        stop();
        myThread.join();
    }
}

void TcpServer::mainLoop()
{
    Logger &logger  = Logger::get("iotool.tcp-server");
    Timer timer;
    TimerTask::Ptr taskPtr(new AcquisitionTask(*this));
    logger.information("Launching TIMER...");
    timer.schedule(taskPtr,Timestamp() /*now*/,mysendInterval);


    logger.debug("Timer started. Entering event loop.");
    while( !myTerminate ) {
        logger.information( "Waiting for new client...");
        SocketAddress clientAddress;
        StreamSocket stream = mySocket.acceptConnection(clientAddress);
        logger.information( format("New client connected from %s",clientAddress.toString() ) );
        myMutex.lock();
        myStreams.push_back(stream);
        myMutex.unlock();
    }
    logger.information("Event loop terminated. Canceling timer task...");
    timer.cancel(true);
}

void TcpServer::start()
{
    if ( myThread.joinable() ) {
        Logger::get("iotool.tcp-server").warning( "TCP server thread already active?! 0.o Bailing out");
        return;
    }
    Logger::get("iotool.tcp-server").information("Launching TCP server worker thread");
    myThread    = std::thread(&TcpServer::mainLoop,this);

}

void TcpServer::stop()
{
    myTerminate     = true;
    myThread.join();
    mySocket.close();
}


