//
// Created by to on 13/04/19.
//

#ifndef IOTOOL_JOB_MANAGER_HPP
#define IOTOOL_JOB_MANAGER_HPP

#include <thread>


#include <common/data_bucket_consumer.hpp>
#include <common/blocking_queue.hpp>
#include "transform_job.hpp"

class TransformJobManager : public DataBucketConsumer
{
private:
    Rpn::RpnLib myRpnLib;
    std::list<TransformJobPtr> myJobs;
    BlockingQueue<DataBucketPtr> myQueue;
    bool myTerminated;
    std::thread myThread;
    std::mutex myMutex;
    std::list<DataBucketConsumer*> myConsumers;

    void workerFunction();

    void p_disptatch(DataBucketPtr bucket);


public:
    TransformJobManager();

    virtual ~TransformJobManager();

    bool loadFromPath( std::string const &path );

    void incomingBucket(DataBucketPtr db) override
    {
        myQueue.enqueue(db);
    }

    void teardown()
    {
        myTerminated    = true;
        myQueue.shutdown();
        myThread.join();
    }


    void addConsumer(DataBucketConsumer *consumer);
};


#endif //IOTOOL_JOB_MANAGER_HPP
