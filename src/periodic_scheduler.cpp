//
// Created by to on 28/03/19.
//

#include <Poco/Util/TimerTaskAdapter.h>
#include <Poco/Logger.h>
#include <Poco/JSON/Object.h>

#include <common/acquisition_scheduler.hpp>

#include "periodic_scheduler.hpp"

using Poco::Logger;
using Poco::format;
using namespace Poco::JSON;
using namespace std;


PeriodicScheduler::PeriodicScheduler(ShellBackendPtr shellBackend) :
logger( Logger::get("iotool") ) ,
myShellBackend(shellBackend)
{

}

PeriodicScheduler::~PeriodicScheduler()
{
    if ( !myTimerTask.isNull() ) {
        logger.information("PeriodicScheduler: timer still running. Stopping...");
        stop();
    }
}

PeriodicScheduler::Ptr PeriodicScheduler::loadFromJSON(Poco::JSON::Object::Ptr config, ShellBackendPtr shellBackend)
{
    Logger &logger = Logger::get("iotool");

    try {
        Ptr scheduler(new PeriodicScheduler(shellBackend));

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

void PeriodicScheduler::start()
{
    if ( myTimerTask.isNull() ) {
        logger.information(format("PeriodicScheduler: starging timer task. interval=%dms",myExecutionInterval));
        myTimerTask = new Poco::Util::TimerTaskAdapter<PeriodicScheduler>(*this,&PeriodicScheduler::onTimer);
        myTimer.scheduleAtFixedRate(myTimerTask,myExecutionInterval,myExecutionInterval);
    }
    else {
        logger.warning("PeriodicScheduler: timer is already running. Ignoring start()");
    }

}

void PeriodicScheduler::stop()
{
    if ( !myTimerTask.isNull() ) {
        logger.information("PeriodicScheduler: stopping timer.");
        myTimerTask->cancel();
        myTimerTask.reset();
    }
    else {
        logger.warning("PeriodicScheduler: timer already stopped. Ignoring stop()");
    }

}

void PeriodicScheduler::onTimer(Poco::Util::TimerTask &)
{
    logger.debug("PeriodicScheduler: running EXECUTION script...");
    ShellScriptRunner::runScript(myExecutionScript,myShellBackend);
}


