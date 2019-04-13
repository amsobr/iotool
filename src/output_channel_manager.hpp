//
// Created by to on 12/04/19.
//

#ifndef IOTOOL_OUTPUT_CHANNEL_MANAGER_HPP
#define IOTOOL_OUTPUT_CHANNEL_MANAGER_HPP


#include <list>
#include <thread>

#include <common/output_channel.hpp>
#include <common/blocking_queue.hpp>

class OutputChannelManager : public DataBucketConsumer
{
private:
    std::list<OutputChannelPtr> myOutputChannels;
    BlockingQueue<DataBucketPtr> myQueue;
    std::thread myThread;
    bool myStopped;

    void workerFunction();

public:
    OutputChannelManager(std::list<OutputChannelPtr> const &channels ) :
    myOutputChannels(channels)  ,
    myQueue()   ,
    myThread(&OutputChannelManager::workerFunction,this)
    {

    }

    ~OutputChannelManager() override;

    void incomingBucket(DataBucketPtr db) override
    {
        myQueue.enqueue(db);
    }

    void close()
    {
        myStopped   = true;
        myQueue.shutdown();
        myThread.join();
    }

};


#endif //IOTOOL_OUTPUT_CHANNEL_MANAGER_HPP
