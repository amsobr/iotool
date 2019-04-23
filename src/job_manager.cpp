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
myRpnLib() ,
myJobs() ,
myQueue() ,
myTerminated(false) ,
myThread(&TransformJobManager::workerFunction,this) ,
myMutex() ,
myConsumers()
{
    myRpnLib.loadAddons(Iotool::RPNLIB_ADDONS_PATH);

}

TransformJobManager::~TransformJobManager()
{

}




void TransformJobManager::workerFunction()
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

    File dir(path);
    if ( !dir.isDirectory() ) {
        logger.error( format("OOPS: '%s' doesn't name a directory...",path));
        return false;
    }

    vector<File> allFiles;
    dir.list(allFiles);
    for ( auto file : allFiles ) {
        if ( !file.isFile() ) {
            logger.information("TransformJobManager: ignoring non-file path: '%s'", file.path());
            continue;
        }
        try {
            TransformJobPtr p( new TransformJob(file.path(),myRpnLib) );
            logger.information( format("loaded transform job from '%s'",file.path()));
            myJobs.push_back(p);
        }
        catch ( exception ex ) {
            logger.warning( format("Failed to load transform job from '%s' - %s",file.path(),ex.what()));
        }
    }
    return true;
}

