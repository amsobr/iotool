//
// Created by to on 13/04/19.
//

#include <Poco/Logger.h>
#include <Poco/File.h>

#include "iotool_config.hpp"

#include "job_manager.hpp"


using Poco::Logger;
using Poco::format;
using Poco::File;
using namespace std;

TransformJobManager::TransformJobManager() :
logger(Logger::get("iotool")) ,
myRpnLib(nullptr) ,
myJobs() ,
myQueue() ,
myTerminated(false) ,
myThread(&TransformJobManager::workerFunction,this) ,
myMutex() ,
myConsumers()
{
}

TransformJobManager::~TransformJobManager()
{
    if ( myRpnLib!=nullptr ) {
        delete myRpnLib;
    }
}




void TransformJobManager::workerFunction()
{
    logger.information("TransformJobManager: worker thread started.");
    while( !myTerminated ) {
        logger.information("TransformJobManager: waiting for incoming buckets...");
        DataBucketPtr bucketPtr = myQueue.getNext();
        logger.information(format("TransformJobManager: received bucket. name='%' numPoints=%z",bucketPtr->name,bucketPtr->dataPoints.size()));
        logger.information(format("TransformJobManager: received bucket is:\n%s",bucketPtr->toString()));

        for (auto job : myJobs) {
            if (job->isCalled(bucketPtr->name)) {
                logger.information("TransformJobManager: processing bucket...");
                DataBucketPtr result = job->processBucket(bucketPtr);
                logger.information(format("TransformJobManager: bucket after transformation:\n%s",result->toString()));
                logger.information("TransformJobManager: dispatching bucket to consumers...");
                p_disptatch(result);
            }
        }
    }
}

void TransformJobManager::addConsumer( DataBucketConsumer *consumer )
{
    lock_guard<mutex> guard(mutex);
    myConsumers.push_back(consumer);
}

void TransformJobManager::p_disptatch(DataBucketPtr bucket)
{
    lock_guard<mutex> guard(myMutex);
    for ( auto consumer : myConsumers ) {
        consumer->incomingBucket(bucket);
    }

}

bool TransformJobManager::loadFromPath(std::string const &path)
{
    Logger &logger  = Logger::get("iotool");

    myRpnLib        = new Rpn::RpnLib();
    myRpnLib->init();
    File rpnLibDir(path+"/rpnlib_addons");
    if ( rpnLibDir.exists() && rpnLibDir.isDirectory() ) {
        logger.information(format("TransformJobManager: loading RPN lib addons from '%s'",rpnLibDir.path()));
        myRpnLib->loadAddons(rpnLibDir.path());
    }

    File dir(path);
    vector<File> allFiles;
    dir.list(allFiles);
    for ( auto file : allFiles ) {
        if ( !file.isFile() ) {
            logger.information("TransformJobManager: ignoring non-file path: '%s'", file.path());
            continue;
        }
        try {
            logger.information( format("TransformJobManager: trying to load job from '%s'" , file.path()));
            TransformJobPtr p( new TransformJob(file.path(),*myRpnLib) );
            logger.information( format("TransformJobManager: loaded transform job from '%s'",file.path()));
            myJobs.push_back(p);
        }
        catch ( exception ex ) {
            logger.error( format("Failed to load transform job from '%s' - %s",file.path(),string(ex.what())));
        }
    }
    return true;
}

