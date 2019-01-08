#ifndef TCP_SERVER_HPP
#define TCP_SERVER_HPP

#include <string>
#include <thread>
#include <list>
#include <mutex>

#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/StreamSocket.h>
#include <Poco/Util/TimerTask.h>

#include <common/board.hpp>

class TcpServer
{
private:
    BoardPtr myBoard;
    std::string myBoundAddress;
    unsigned int myBoundPort;
    unsigned int mysendInterval;
    std::thread myThread;
    std::list<Poco::Net::StreamSocket> myStreams;
    std::mutex myMutex;
    bool myTerminate;
    Poco::Net::ServerSocket mySocket;

    void mainLoop();

    class AcquisitionTask : public Poco::Util::TimerTask
    {
        private:
            TcpServer &self;

        public:
            AcquisitionTask(TcpServer &server);
            virtual ~AcquisitionTask();

            virtual void run();
    };


public:
    TcpServer( BoardPtr board , unsigned int port , unsigned int interval );
    ~TcpServer();

    void start();
    void stop();
    bool isRunning();


}; /* class TcpServer */



#endif /* !defined TCP_SERVER_HPP */
