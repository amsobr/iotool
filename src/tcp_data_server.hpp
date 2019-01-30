#ifndef TCP_DATA_SERVER_HPP
#define TCP_DATA_SERVER_HPP

#include <string>
#include <thread>
#include <vector>
#include <list>
#include <mutex>
#include <condition_variable>
#include <memory>

#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/StreamSocket.h>
#include <Poco/Logger.h>


#include <common/data_bucket_consumer.hpp>

class TcpDataServer : public DataBucketConsumer
{
private:
    std::string myBoundAddress;
    unsigned int myBoundPort;
    unsigned int mysendInterval;
    std::thread myAcceptorThread;
    std::thread myDispatcherThread;
    std::list<Poco::Net::StreamSocket> myStreams;
    std::mutex myMutex;
    std::condition_variable myCondition;
    bool myTerminate;
    Poco::Net::ServerSocket mySocket;
    std::vector<DataBucket> myQueuedBuckets;
    Poco::Logger &logger;

    void dispatcherLoop();
    void acceptorLoop();

public:
    TcpDataServer( unsigned int port );
    ~TcpDataServer();

    void start();
    void stop();
    bool isRunning();

    virtual void processBucket( DataBucket const &db );

}; /* class TcpDataServer */



#endif /* !defined TCP_DATA_SERVER_HPP */
