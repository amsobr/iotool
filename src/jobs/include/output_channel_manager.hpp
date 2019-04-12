//
// Created by to on 20/03/19.
//

#ifndef IOTOOL_OUTPUT_CHANNEL_MANAGER_HPP
#define IOTOOL_OUTPUT_CHANNEL_MANAGER_HPP


#include <common/data_bucket_consumer.hpp>
#include <common/output_channel.hpp>
#include <common/blocking_queue.hpp>
#include <thread>

class OutputChannelManager : public DataBucketConsumer
{
private:
    std::mutex myMutex;
    std::list<OutputChannelPtr> myChannels;
    bool myQueueEnabled;
    BlockingQueue<DataBucketPtr> myQueue;
    std::thread myThread;
    bool myShutdown;

public:
    OutputChannelManager( bool enableQueue) :
    myMutex() ,
    myQueueEnabled(enableQueue) ,
    myShutdown(false)
    {
        if ( enableQueue ) {
            myThread    = std::thread(&OutputChannelManager::worker,this);
        }

    }

    virtual ~OutputChannelManager() {}


    std::list<OutputChannelPtr> getChannelsByName(std::string name)
    {
        std::lock_guard<std::mutex> lock(myMutex);

        std::list<OutputChannelPtr> ret;
        for ( auto ch : myChannels ) {
            if ( ch->getName()==name ) {
                ret.push_back(ch);
            }
        }
        return ret;
    }

    void addChannel( OutputChannelPtr ch )
    {
        std::lock_guard<std::mutex> lock(myMutex);
        myChannels.push_back(ch);
    }

    void processBucket(DataBucketPtr db)
    {
        std::string name    = db->name;
        std::list<OutputChannelPtr> channels    = getChannelsByName(name);

        for ( auto ch : channels ) {
            ch->incomingBucket(db);
        }
    }

    void worker()
    {
        while( !myShutdown ) {
            DataBucketPtr db    = myQueue.getNext();
            processBucket(db);
        }

    }

    void incomingBucket(DataBucket &db) override
    {
        if ( myQueueEnabled ) {
            myQueue.enqueue(db);
        }
        else {
            processBucket(db);
        }
    }

    void shutdown()
    {
        if ( myQueueEnabled ) {
            myQueue.shutdown();
            myShutdown  = true;
            myThread.join();
        }
    }


};


#endif //IOTOOL_OUTPUT_CHANNEL_MANAGER_HPP
