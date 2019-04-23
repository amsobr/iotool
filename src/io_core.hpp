//
// Created by to on 19/04/19.
//

#ifndef IOTOOL_IO_CORE_HPP
#define IOTOOL_IO_CORE_HPP


#include <shell/shell_backend.hpp>
#include <shell/shell_frontend.hpp>
#include <common/acquisition_scheduler.hpp>

#include "job_manager.hpp"
#include "output_channel_manager.hpp"
#include "periodic_scheduler.hpp"

class IoCore
{
private:
    std::vector<AcquisitionScheduler::Ptr> mySchedulers;
    TransformJobManager *myDataConverters;
    OutputChannelManager *myOutputStage;
    ShellBackendPtr myShellBackend;
    DataBucket myAccumulator;

public:
    IoCore();

    virtual ~IoCore();


    void shutdown();

};


#endif //IOTOOL_IO_CORE_HPP
