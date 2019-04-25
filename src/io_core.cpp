//
// Created by to on 19/04/19.
//

#include <string>

#include <Poco/Logger.h>
#include <Poco/Format.h>
#include <Poco/File.h>

#include <shell/shell_backend_factory.hpp>
#include "io_core.hpp"
#include "iotool_config.hpp"
#include "acquisition_scheduler_factory.hpp"

using namespace std;
using Poco::Logger;
using Poco::format;
using Poco::File;

static inline void ensureDirExists(string const &path, string const &role, Logger &logger)
{
    File dir(path);
    if ( !dir.exists() ) {
        logger.error(format("IoCore: %s dir '%s' does not exist.",role,path));
        throw std::runtime_error("IoCore " + role + " config dir does not exist: " + path );
    }
    if ( !dir.isDirectory() ) {
        logger.error(format("IoCore: %s dir '%s' isn't actually a directory!",path));
        throw std::runtime_error("IoCore " + role + " configdir is not a directory: " + path );
    }
}

IoCore::IoCore( string configDirPath ) :
logger( Logger::get("iotool") ) ,
myShellBackend( ShellBackendFactory::createInstance() ) ,
myConfigDirPath(configDirPath)
{
    logger.information(format("IoCore: loading configuration from '%s'...",configDirPath));

    ensureDirExists(myConfigDirPath,"ConfigBase",logger);

    string schedulersPath  = myConfigDirPath+"/"+Iotool::SCHEDULERS_PATH;
    ensureDirExists(schedulersPath,"SchedulersConfig",logger);

    string dataConvertersPath   = myConfigDirPath+"/"+Iotool::TRANSFORM_JOBS_PATH;
    ensureDirExists(dataConvertersPath,"DataConvertersConfig",logger);

    string outputStagePath = myConfigDirPath+"/"+Iotool::OUTPUT_CHANNELS_PATH;
    ensureDirExists(outputStagePath,"OutputStageConfig",logger);

    logger.information( format("IoCore: Loading Acquisition schedulers from '%s'...",schedulersPath));
    mySchedulers    = AcquisitionSchedulerFactory::loadFromPath(schedulersPath,myShellBackend);

    logger.information( format("IoCore: Loading Transform Jobs from '%s'...",dataConvertersPath));
    myDataConverters    = new TransformJobManager();
    myDataConverters->loadFromPath(dataConvertersPath);

    logger.information( format("IoCore: Loading output channels from '%s'",outputStagePath));
    myOutputStage       = new OutputChannelManager();
    myOutputStage->loadFromPath(outputStagePath);

    /* do all the wiring... */
    logger.information("IoCore: setting up callbacks...");
    myDataConverters->addConsumer(myOutputStage);
    myShellBackend->addConsumer(myDataConverters);
    logger.information("IoCore: Core finished loading.");
}

IoCore::~IoCore()
{

}

void IoCore::shutdown()
{

}

void IoCore::start()
{
    for ( auto scheduler : mySchedulers ) {
        scheduler->start();
    }

}
