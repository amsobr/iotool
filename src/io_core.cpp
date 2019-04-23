//
// Created by to on 19/04/19.
//

#include <shell/shell_backend_factory.hpp>
#include "io_core.hpp"
#include "iotool_config.hpp"
#include "acquisition_scheduler_factory.hpp"

IoCore::IoCore() :
myShellBackend( ShellBackendFactory::createInstance() )
{
    mySchedulers    = AcquisitionSchedulerFactory::loadFromPath(Iotool::SCHEDULERS_PATH,myShellBackend);

    myDataConverters    = new TransformJobManager();
    myDataConverters->loadFromPath(Iotool::TRANSFORM_JOBS_PATH);
    myOutputStage       = new OutputChannelManager();
    myOutputStage->loadFromPath(Iotool::OUTPUT_CHANNELS_PATH);

    /* do all the wiring... */
    myDataConverters->addConsumer(myOutputStage);
    myShellBackend->addConsumer(myDataConverters);

}

IoCore::~IoCore()
{

}

void IoCore::shutdown()
{

}
