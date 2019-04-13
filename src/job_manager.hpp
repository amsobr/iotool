//
// Created by to on 13/04/19.
//

#ifndef IOTOOL_JOB_MANAGER_HPP
#define IOTOOL_JOB_MANAGER_HPP

#include <thread>


#include <common/data_bucket_consumer.hpp>
#include <common/blocking_queue.hpp>
#include "jobs/include/job.hpp"

class JobManager : public DataBucketConsumer
{
private:
    std::list<JobPtr> myJobs;
    BlockingQueue<DataBucketPtr> myQueue;
    bool myTerminated;
    std::thread myThread;
    std::mutex myMutex;
    std::list<DataBucketConsumer*> myConsumers;

    void workerFunction();

    void p_disptatch(DataBucketPtr bucket);


public:
    JobManager( std::list<JobPtr> &jobs );

    virtual ~JobManager();

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
