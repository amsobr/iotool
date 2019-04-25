//
// Created by to on 12/04/19.
//

#ifndef IOTOOL_OUTPUT_CHANNEL_MANAGER_HPP
#define IOTOOL_OUTPUT_CHANNEL_MANAGER_HPP


#include <list>
#include <thread>

#include <Poco/Logger.h>

#include <common/output_channel.hpp>
#include <common/blocking_queue.hpp>
#include "output_channel_holder.hpp"

class OutputChannelManager : public DataBucketConsumer
{
private:
    Poco::Logger &logger;
    std::list<OutputChannelHolder> myOutputChannels;
    BlockingQueue<DataBucketPtr> myQueue;
    bool myStopped;
    std::thread myThread;

    void workerFunction();

public:
    OutputChannelManager() :
    logger(Poco::Logger::get("iotool")) ,
    myOutputChannels()  ,
    myQueue()   ,
    myStopped(false) ,
    myThread(&OutputChannelManager::workerFunction,this)
    {

    }

    ~OutputChannelManager() override = default;

    void incomingBucket(DataBucketPtr db) override;

    void close();

    bool loadFromPath(std::string const &path);
};


#endif //IOTOOL_OUTPUT_CHANNEL_MANAGER_HPP
