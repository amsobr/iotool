//
// Created by to on 12/04/19.
//

#ifndef IOTOOL_OUTPUT_CHANNEL_MANAGER_HPP
#define IOTOOL_OUTPUT_CHANNEL_MANAGER_HPP


#include <list>
#include <thread>

#include <common/output_channel.hpp>
#include <common/blocking_queue.hpp>
#include "output_channel_holder.hpp"

class OutputChannelManager : public DataBucketConsumer
{
private:
    std::list<OutputChannelHolder> myOutputChannels;
    BlockingQueue<DataBucketPtr> myQueue;
    std::thread myThread;
    bool myStopped;

    void workerFunction();

public:
    OutputChannelManager() :
    myOutputChannels()  ,
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

    bool loadFromPath(std::string const &path);
};


#endif //IOTOOL_OUTPUT_CHANNEL_MANAGER_HPP
