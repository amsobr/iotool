//
// Created by to on 28/03/19.
//

#include <common/acquisition_scheduler.hpp>
#include <Poco/Logger.h>
#include <Poco/JSON/Object.h>
#include "periodic_scheduler.hpp"

using Poco::Logger;
using Poco::format;
using namespace Poco::JSON;
using namespace std;

PeriodicScheduler::PeriodicScheduler()
{

}

PeriodicScheduler::~PeriodicScheduler()
{

}

PeriodicScheduler::Ptr PeriodicScheduler::loadFromJSON(Poco::JSON::Object::Ptr config)
{
    Logger &logger = Logger::get("iotool");

    try {
        Ptr scheduler(new PeriodicScheduler());

        logger.information("Loading PeriodicScheduler...");


        logger.information("Loading 'setupScript'...");
        Array::Ptr jsonSetupScript = config->getArray("setupScript");
        for (auto it = jsonSetupScript->begin(); it != jsonSetupScript->end(); it++) {
            scheduler->mySetupScript.push_back((*it).convert<string>());
        }

        logger.information("Loading 'executionScript'...");
        Array::Ptr jsonExecScript = config->getArray("executionScript");
        for (auto it = jsonExecScript->begin(); it != jsonExecScript->end(); it++) {
            scheduler->myExecutionScript.push_back((*it).convert<string>());
        }

        logger.information("Loading 'teardownScript'...");
        Array::Ptr jsonTeardownScript = config->getArray("teardownScript");
        for (auto it = jsonTeardownScript->begin(); it != jsonTeardownScript->end(); it++) {
            scheduler->myTeardownScript.push_back((*it).convert<string>());
        }

        logger.information("Loading 'executionInterval'...");
        scheduler->myExecutionInterval = config->getValue<unsigned long>("executionInterval");

        logger.information("Things are looking good. Scheduler was loaded.");
        return scheduler;
    }
    catch ( std::exception ex )
    {
        logger.error( format("Something went wrong loading this scheduler from JSON: %s",ex.what()));
    }
    return nullptr;
}
