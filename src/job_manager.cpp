//
// Created by to on 13/04/19.
//

#include "job_manager.hpp"


using namespace std;

JobManager::JobManager( list<JobPtr> &jobs ) :
myJobs(jobs) ,
myQueue() ,
myTerminated(false) ,
myThread(&JobManager::workerFunction,this) ,
myMutex() ,
myConsumers()
{

}

JobManager::~JobManager()
{

}


void JobManager::workerFunction()
{
    while( !myTerminated ) {
        DataBucketPtr bucketPtr = myQueue.getNext();

        for (auto job : myJobs) {
            if (job->isCalled(bucketPtr->name)) {
                DataBucketPtr result = job->processBucket(bucketPtr);
                p_disptatch(result);
            }
        }
    }
}

void JobManager::addConsumer( DataBucketConsumer *consumer )
{
    lock_guard<mutex> guard(mutex);
    myConsumers.push_back(consumer);
}

void JobManager::p_disptatch(DataBucketPtr bucket)
{
    lock_guard<mutex> guard(myMutex);
    for ( auto consumer : myConsumers ) {
        consumer->incomingBucket(bucket);
    }

}


